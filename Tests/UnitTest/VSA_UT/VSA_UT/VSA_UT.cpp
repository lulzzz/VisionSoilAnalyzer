#define BOOST_TEST_MODULE VSA_UNIT_TESTS

// Custom libraries
#include "../../../../src/VisionSoilAnalyzer/Vision/Vision.h"
#include "../../../../src/VisionSoilAnalyzer/Soil/VisionSoil.h"
#include "../../../../src/VisionSoilAnalyzer/SoilMath/SoilMath.h"
#include "FloatTestMatrix.h"
#include "TestMatrix.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/results_reporter.hpp>
#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

// Statistical analysis
#include "StatisticalComparisonDefinition.h"

#include <math.h>
#include <cmath>
#include <random>
#include <sys/time.h>

using namespace cv;
using namespace std;

// Create the Report Redirector
struct LogToFile
{
	LogToFile()
	{
		std::string logFileName(boost::unit_test::framework::master_test_suite().p_name);
		logFileName.append(".log");
		logFile.open(logFileName.c_str());
		boost::unit_test::unit_test_log.set_stream(logFile);
	}
	~LogToFile()
	{
		boost::unit_test::unit_test_log.test_finish();
		logFile.close();
		boost::unit_test::unit_test_log.set_stream(std::cout);
	}
	std::ofstream logFile;
};
BOOST_GLOBAL_FIXTURE(LogToFile);

struct M {
	M() 
	{
		BOOST_TEST_MESSAGE("setup fixture");
		src = imread("../ComparisionPictures/SoilSampleRGB.ppm"); 
	}
	~M()         { BOOST_TEST_MESSAGE("teardown fixture"); }

	Mat src;
	Mat dst;
	Mat comp;
};

//----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(SoilMath_Test_Suit)

BOOST_AUTO_TEST_CASE(SoilMath_Sort)
{
	int testDiscrete[] = { 2, 8, 1, 3, 0, 7, 4, 5, 9, 6, 6 };
	int ucomp[] = { 0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 9};
	SoilMath::Sort::QuickSort<int>(testDiscrete, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(testDiscrete, testDiscrete + 10, ucomp, ucomp + 10);
}

BOOST_AUTO_TEST_CASE(SoilMath_ucharStat_t)
{
	ucharStat_t Test((uint8_t *)testMatrix, 200, 200);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.bins, Test.bins + 255, histTestResult, histTestResult + 255);
	BOOST_CHECK_CLOSE(Test.Mean, meanTestResult, 0.0001);
	BOOST_CHECK_EQUAL(Test.n, nTestResult);
	BOOST_CHECK_EQUAL(Test.Sum, sumTestResult);
	BOOST_CHECK_EQUAL(Test.min, minTestResult);
	BOOST_CHECK_EQUAL(Test.max, maxTestResult);
	BOOST_CHECK_EQUAL(Test.Range, rangeTestResult);
	BOOST_CHECK_CLOSE(Test.Std, stdTestResult, 0.01);
}

BOOST_AUTO_TEST_CASE(SoilMath_floatStat_t)
{
	floatStat_t Test((float *)ftestMatrix, 50, 50);

	BOOST_CHECK_EQUAL_COLLECTIONS(Test.bins, Test.bins + 255, fhistTestResult, fhistTestResult + 255);
	BOOST_CHECK_CLOSE(Test.Mean, fmeanTestResult, 0.01);
	BOOST_CHECK_EQUAL(Test.n, fnTestResult);
	BOOST_CHECK_CLOSE(Test.Sum, fsumTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.min, fminTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.max, fmaxTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.Range, frangeTestResult, 0.01);
	BOOST_CHECK_CLOSE(Test.Std, fstdTestResult, 0.025);
}

BOOST_AUTO_TEST_CASE(SoilMath_FFT_GetDescriptors)
{
	uchar data[] =
	{ 0, 0, 1, 1, 0, 0,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 0, 0 };
	cv::Mat src(8, 6, CV_8UC1, &data, 1);
	SoilMath::FFT Test;
	ComplexVect_t desc = Test.GetDescriptors(src);
	Complex_t desc_exp[] = {
		Complex_t(-1.6666667, -6),
		Complex_t(2.02375780, -0.16742019),
		Complex_t(-3.1094758, -6.13316500),
		Complex_t(-1.8036530, 1.023864110),
		Complex_t(-0.6666667, -1),
		Complex_t(0.04350554, -2.55884126),
		Complex_t(-3.0522848, -0.63807119),
		Complex_t(-0.7780360, -0.31809187),
		Complex_t(-0.3333333, 0),
		Complex_t(-1.3856866, -1.43021101),
		Complex_t(-1.2238576, 0.466498316),
		Complex_t(-0.3295119, 1.459385072),
		Complex_t(1.33333331, -1),
		Complex_t(-0.3482434, 0.489805636),
		Complex_t(0.71895134, 0.304737878),
		Complex_t(5.24453433, -0.49849056)
	};

	BOOST_CHECK_EQUAL(desc.size(), 16);

	for (uint32_t i = 0; i < 16; i++)
	{
		BOOST_CHECK_CLOSE(desc[i].real(), desc_exp[i].real(), 0.0001);
		BOOST_CHECK_CLOSE(desc[i].imag(), desc_exp[i].imag(), 0.0001);
	}
}

BOOST_AUTO_TEST_CASE(SoilMath_FFT_GetDescriptors_Non_Continues_Contour)
{
	uchar data[] =
	{ 0, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	1, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 0, 0 };
	cv::Mat src(8, 6, CV_8UC1, &data, 1);
	SoilMath::FFT Test;
	BOOST_CHECK_THROW(Test.GetDescriptors(src), SoilMath::Exception::MathException);
}

BOOST_AUTO_TEST_CASE(SoilMath_NN_Save_And_Load)
{
	SoilMath::NN Test(3, 5, 2);

	InputLearnVector_t inputVect;
	OutputLearnVector_t outputVect;

	//Population_t pop;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float i1 = 0.0, i2 = 0.0, i3 = 0.0;
	float o1 = 0.0, o2 = 0.0;

	for (uint32_t i = 0; i < 200; i++)
	{
		if (dis(gen) > 0.5f) { i1 = 1.0; }
		else { i1 = 0.0; }
		if (dis(gen) > 0.5f) { i2 = 1.0; }
		else { i2 = 0.0; }
		if (dis(gen) > 0.5f) { i3 = 1.0; }
		else { i3 = 0.0; }

		if (i1 == 1.0 && i2 == 1.0 && i3 == 0.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else if (i1 == 0.0 && i2 == 0.0 && i3 == 1.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else
		{
			o1 = -1.0;
			o2 = 1.0;
		}

		ComplexVect_t inputTemp;
		inputTemp.push_back(Complex_t(i1, 0));
		inputTemp.push_back(Complex_t(i2, 0));
		inputTemp.push_back(Complex_t(i3, 0));
		inputVect.push_back(inputTemp);

		Predict_t outputTemp;
		outputTemp.OutputNeurons.push_back(o1);
		outputTemp.OutputNeurons.push_back(o2);
		outputVect.push_back(outputTemp);
	}

	Test.Learn(inputVect, outputVect, 0);
	Test.SaveState("NN.xml");

	SoilMath::NN loadTest;
	loadTest.LoadState("NN.xml");

	std::vector<float> test_out = Test.Predict(inputVect[0]).OutputNeurons;
	std::vector<float> loadtest_out = loadTest.Predict(inputVect[0]).OutputNeurons;

	BOOST_REQUIRE_EQUAL_COLLECTIONS(Test.hWeights.begin(), Test.hWeights.end(), loadTest.hWeights.begin(), loadTest.hWeights.end());
	BOOST_REQUIRE_EQUAL_COLLECTIONS(Test.iWeights.begin(), Test.iWeights.end(), loadTest.iWeights.begin(), loadTest.iWeights.end());
}

BOOST_AUTO_TEST_CASE(SoilMath_NN_Prediction_Accurancy)
{
	SoilMath::NN Test;
	Test.LoadState("NN.xml");


	InputLearnVector_t inputVect;
	OutputLearnVector_t outputVect;
	OutputLearnVector_t outputPredictVect;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float i1 = 0.0, i2 = 0.0, i3 = 0.0;
	float o1 = 0.0, o2 = 0.0;

	for (uint32_t i = 0; i < 10; i++)
	{
		if (dis(gen) > 0.5f) { i1 = 1.0; }
		else { i1 = 0.0; }
		if (dis(gen) > 0.5f) { i2 = 1.0; }
		else { i2 = 0.0; }
		if (dis(gen) > 0.5f) { i3 = 1.0; }
		else { i3 = 0.0; }

		if (i1 == 1.0 && i2 == 1.0 && i3 == 0.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else if (i1 == 0.0 && i2 == 0.0 && i3 == 1.0)
		{
			o1 = 1.0;
			o2 = -1.0;
		}
		else
		{
			o1 = -1.0;
			o2 = 1.0;
		}

		ComplexVect_t inputTemp;
		inputTemp.push_back(Complex_t(i1, 0));
		inputTemp.push_back(Complex_t(i2, 0));
		inputTemp.push_back(Complex_t(i3, 0));
		inputVect.push_back(inputTemp);

		Predict_t outputTemp;
		outputTemp.OutputNeurons.push_back(o1);
		outputTemp.OutputNeurons.push_back(o2);
		outputVect.push_back(outputTemp);

		Predict_t outputPredictTemp;
		outputPredictTemp.OutputNeurons = Test.Predict(inputTemp).OutputNeurons;
		
		for (uint32_t j = 0; j < outputTemp.OutputNeurons.size(); j++)
		{
			BOOST_CHECK_CLOSE(outputPredictTemp.OutputNeurons[j], outputTemp.OutputNeurons[j], 5);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(Vision_Test_Suite)

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_Intensity, M)
{
	// Convert the RGB picture to an intensity picture
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::Intensity);

	// Read in the Matlab converted intensity picture converted with the Matlab command:
	// Matlab_int=0.2126*RGB(:,:,1)+0.7152*RGB(:,:,2)+0.0722*RGB(:,:,3);
	comp = imread("../ComparisionPictures/Matlab_int.ppm", 0);

	// Calculate the statistics of the two images
	ucharStat_t statDst(dst.data, dst.rows, dst.cols);
	ucharStat_t statComp(comp.data, comp.rows, comp.cols);

	// Simple comparison
	BOOST_CHECK_CLOSE(statDst.Mean, statComp.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDst.Std, statComp.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.Range, (double)statComp.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.min, (double)statComp.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.max, (double)statComp.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDst.Sum, (double)statComp.Sum, 0.5);

	// Welch test comparison of the means
	bool rejected = statDst.WelchTest(statComp);
	BOOST_CHECK_EQUAL(rejected, true);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_CIEXYZ, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::CIE_XYZ);
	vector<Mat> LAB = Test.extractChannel(dst);

	floatStat_t statDstX((float *)LAB[0].data, src.rows, src.cols);
	floatStat_t statCompX;
	statCompX.Std = X_STD;
	statCompX.n = N_MAT;
	statCompX.Mean = X_MEAN;
	statCompX.Range = X_RANGE;
	statCompX.min = X_MIN;
	statCompX.max = X_MAX;
	statCompX.Sum = X_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstX.Mean, statCompX.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstX.Std, statCompX.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.Range, (double)statCompX.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.min, (double)statCompX.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.max, (double)statCompX.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstX.Sum, (double)statCompX.Sum, 0.5);

	//// Welch test comparison of the means
	bool rejected = statDstX.WelchTest(statCompX);
	BOOST_CHECK_EQUAL(rejected, false); // TODO: Find out why my null hypothese doesn't hold

	floatStat_t statDstY((float *)LAB[1].data, src.rows, src.cols);
	floatStat_t statCompY;
	statCompY.Std = Y_STD;
	statCompY.n = N_MAT;
	statCompY.Mean = Y_MEAN;
	statCompY.Range = Y_RANGE;
	statCompY.min = Y_MIN;
	statCompY.max = Y_MAX;
	statCompY.Sum = Y_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstY.Mean, statCompY.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstY.Std, statCompY.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.Range, (double)statCompY.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.min, (double)statCompY.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.max, (double)statCompY.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstY.Sum, (double)statCompY.Sum, 0.5);

	//// Welch test comparison of the means
	rejected = statDstY.WelchTest(statCompY);
	BOOST_CHECK_EQUAL(rejected, false);

	floatStat_t statDstZ((float *)LAB[2].data, src.rows, src.cols);
	floatStat_t statCompZ;
	statCompZ.Std = Z_STD;
	statCompZ.n = N_MAT;
	statCompZ.Mean = Z_MEAN;
	statCompZ.Range = Z_RANGE;
	statCompZ.min = Z_MIN;
	statCompZ.max = Z_MAX;
	statCompZ.Sum = Z_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstZ.Mean, statCompZ.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstZ.Std, statCompZ.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.Range, (double)statCompZ.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.min, (double)statCompZ.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.max, (double)statCompZ.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstZ.Sum, (double)statCompZ.Sum, 0.5);

	//// Welch test comparison of the means
	rejected = statDstZ.WelchTest(statCompZ);
	BOOST_CHECK_EQUAL(rejected, false);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_RGB_To_CIElab, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, dst, Vision::Conversion::RGB, Vision::Conversion::CIE_lab);
	vector<Mat> LAB = Test.extractChannel(dst);

	floatStat_t statDstL((float *)LAB[0].data, src.rows, src.cols);
	floatStat_t statCompL;
	statCompL.Std = L_STD;
	statCompL.n = N_MAT;
	statCompL.Mean = L_MEAN;
	statCompL.Range = L_RANGE;
	statCompL.min = L_MIN;
	statCompL.max = L_MAX;
	statCompL.Sum = L_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstL.Mean, statCompL.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstL.Std, statCompL.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.Range, (double)statCompL.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.min, (double)statCompL.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.max, (double)statCompL.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstL.Sum, (double)statCompL.Sum, 0.5);

	// Welch test comparison of the means
	bool rejected = statDstL.WelchTest(statCompL);
	BOOST_CHECK_EQUAL(rejected, false);

	floatStat_t statDstA((float *)LAB[1].data, src.rows, src.cols);
	floatStat_t statCompA;
	statCompA.Std = A_STD;
	statCompA.n = N_MAT;
	statCompA.Mean = A_MEAN;
	statCompA.Range = A_RANGE;
	statCompA.min = A_MIN;
	statCompA.max = A_MAX;
	statCompA.Sum = A_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstA.Mean, statCompA.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstA.Std, statCompA.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.Range, (double)statCompA.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.min, (double)statCompA.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.max, (double)statCompA.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstA.Sum, (double)statCompA.Sum, 0.5);

	// Welch test comparison of the means
	rejected = statDstA.WelchTest(statCompA);
	BOOST_CHECK_EQUAL(rejected, false);

	floatStat_t statDstB((float *)LAB[2].data, src.rows, src.cols);
	floatStat_t statCompB;
	statCompB.Std = B_STD;
	statCompB.n = N_MAT;
	statCompB.Mean = B_MEAN;
	statCompB.Range = B_RANGE;
	statCompB.min = B_MIN;
	statCompB.max = B_MAX;
	statCompB.Sum = B_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstB.Mean, statCompB.Mean, 0.5);
	BOOST_CHECK_CLOSE(statDstB.Std, statCompB.Std, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.Range, (double)statCompB.Range, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.min, (double)statCompB.min, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.max, (double)statCompB.max, 0.5);
	BOOST_CHECK_CLOSE((double)statDstB.Sum, (double)statCompB.Sum, 0.5);

	// Welch test comparison of the means
	rejected = statDstB.WelchTest(statCompB);
	BOOST_CHECK_EQUAL(rejected, false);
}

BOOST_FIXTURE_TEST_CASE(Vision_Convert_LAB_To_RI, M)
{
	// Convert the RGB to an CIElab
	Vision::Conversion Test;
	Test.Convert(src, src, Vision::Conversion::RGB, Vision::Conversion::CIE_lab);

	Test.Convert(src, dst, Vision::Conversion::CIE_lab, Vision::Conversion::RI);

	floatStat_t statDstRI((float *)dst.data, src.rows, src.cols);
	floatStat_t statCompRI;
	statCompRI.Std = RI_STD;
	statCompRI.n = N_MAT;
	statCompRI.Mean = RI_MEAN;
	statCompRI.Range = RI_RANGE;
	statCompRI.min = RI_MIN;
	statCompRI.max = RI_MAX;
	statCompRI.Sum = RI_SUM;

	// Simple comparison
	BOOST_CHECK_CLOSE(statDstRI.Mean, statCompRI.Mean, 1.25);
	BOOST_CHECK_CLOSE(statDstRI.Std, statCompRI.Std, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.Range, (double)statCompRI.Range, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.min, (double)statCompRI.min, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.max, (double)statCompRI.max, 1.25);
	BOOST_CHECK_CLOSE((double)statDstRI.Sum, (double)statCompRI.Sum, 1.25);

	// Welch test comparison of the means
	bool rejected = statDstRI.WelchTest(statCompRI);
	BOOST_CHECK_EQUAL(rejected, false);
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(SoilAnalyzer_Test_Suite)

BOOST_FIXTURE_TEST_CASE(Soil_Sample_Save_And_Load, M)
{

	SoilAnalyzer::Sample Test(src);
	Test.Analyse();
	std::string filename = "SoilSample.vsa";
	Test.Save(filename);

	SoilAnalyzer::Sample TestLoad;
	TestLoad.Load(filename);

	//BOOST_CHECK_EQUAL_COLLECTIONS(Test.RGB.datastart, Test.RGB.dataend, TestLoad.RGB.datastart, TestLoad.RGB.dataend);
}

BOOST_AUTO_TEST_SUITE_END()
