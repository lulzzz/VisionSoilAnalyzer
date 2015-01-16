function [Y,Xf,Af] = NN10fit(X,~,~)
%MYNEURALNETWORKFUNCTION neural network simulation function.
%
% Generated by Neural Network Toolbox function genFunction, 27-Oct-2014 20:34:22.
% 
% [Y] = myNeuralNetworkFunction(X,~,~) takes these arguments:
% 
%   X = 1xTS cell, 1 inputs over TS timsteps
%   Each X{1,ts} = 12xQ matrix, input #1 at timestep ts.
% 
% and returns:
%   Y = 1xTS cell of 1 outputs over TS timesteps.
%   Each Y{1,ts} = 1xQ matrix, output #1 at timestep ts.
% 
% where Q is number of samples (or series) and TS is the number of timesteps.

%#ok<*RPMT0>

  % ===== NEURAL NETWORK CONSTANTS =====
  
  % Input 1
  x1_step1_xoffset = [-2.94419709373991;-4.93578418199331;-7.14722479715953;-9.17218483204502;-9.22675680067379;-9.99074323931212;-2.94419709373991;-3.12098866416227;-3.0921128887122;-3.13342526895758;-3.13324332635167;-3.13563842705319];
  x1_step1_gain = [0.328634422652795;0.187719394841059;0.132355358259004;0.105088624960608;0.104987518455396;0.0953759325227081;0.328634422652795;0.320043475892485;0.32260501637595;0.319566156134893;0.323599411788973;0.320434216422146];
  x1_step1_ymin = -1;
  
  % Layer 1
  b1 = [-0.007795264230539372;0.0013016657607355956;-0.35627577048652159;-0.059224855003576909;-0.005577102887277266;0.026052762490403933;-0.0036739204526720012;-0.0086481177918303637;0.0007434243722951819;-0.0036337199961217798];
  IW1_1 = [-0.0012326299582218689 0.020406162713259582 -0.030758358305940623 0.0031792967249101058 -0.012127715684245271 -0.0026089476736003876 -0.0012326297389990441 -0.0078096787940040123 -0.014762078054768313 -0.0057078913563760394 0.0024024875176714977 0.0075690782376974727;0.00023643712249343959 -0.0032289240829041282 0.004929437874788797 -0.00045661733513477267 0.001978062718139429 0.00043826936178944544 0.00023643752817434355 0.0012546316033164896 0.0024278786321387195 0.00090935546916957226 -0.00033846963955325854 -0.0011724990059288464;0.023280491992864111 0.18790872706081782 -0.070556673440710621 0.13290715768011854 0.045947358939519067 0.071301442765149797 0.023280491459614541 0.13853115511713679 -0.19781304495691387 -0.028493448672713168 0.21650976632798385 0.18903483083950159;0.0095820429658889095 0.060678277026584682 -0.13645931334809236 -0.018494009355321344 -0.082553783194600328 -0.04026009953373922 0.0095820430916370852 -0.052259647888954958 -0.052274865625698927 0.027669679899074429 0.022166518266479699 -0.016718731405573905;0.0021276999476043182 0.018763973921880568 -0.022173111430608516 0.0033767634274164439 -0.0077429747095064567 -0.00062663649586797328 0.002127700255619619 -0.0064582816278536855 -0.013556741301727789 -0.0056225194534195999 0.00046547715549987226 0.0048293968852266903;-0.0032607437503295832 -0.044328759476463378 0.065367099503402193 -0.0054637031865455155 0.026946515422380787 0.0065043322753856013 -0.0032607442721872895 0.018340159551356788 0.033074017879992486 0.006717519838293759 -0.0071063205120010711 -0.013410182141046112;-0.00065820955532767153 0.0091265966408206767 -0.013932146948870312 0.0012825720963150176 -0.0056101659346555208 -0.0012535601807744752 -0.00065820927003880825 -0.0035827408047076396 -0.0068226828931025364 -0.0025221735409382846 0.00094051971345620071 0.0033016721280932221;-0.0017520112081290005 0.022135296803187769 -0.03367544504747999 0.0031107728999236082 -0.013487030715912854 -0.0028610289105141865 -0.0017520111818269749 -0.0085457819460077016 -0.0165525805739637 -0.0063645353977468608 0.0019721550388745353 0.007659146716605239;0.00012419053818718388 -0.0018524240413146449 0.002825507989932237 -0.00025741533287732617 0.0011431826858956315 0.00025693737163320256 0.00012419021246814631 0.0007434856871334659 0.0013724574324760065 0.00049098434307416443 -0.00018234241489051132 -0.00065916048874347733;-0.00059268599227057638 0.0089863836274270935 -0.013683827163155499 0.0012607703400802181 -0.0055105974149510318 -0.001221386162778523 -0.00059268592311066378 -0.0035631514583323257 -0.0066857048597887079 -0.0024237214160091562 0.0009149378403869677 0.0032115821100269226];
  
  % Layer 2
  b2 = 0.10633784022831178;
  LW2_1 = [-0.030634345913164288 -0.0029553919226441572 -0.32946919736416008 -0.078125197958109655 -0.069196687395869358 0.048313082789087858 -0.003495200403993641 -0.026657222385634192 0.0051087592457176485 -0.014571984412343315];
  
  % Output 1
  y1_step1_ymin = -1;
  y1_step1_gain = 2.36261788857763;
  y1_step1_xoffset = 0.124235429239349;
  
  % ===== SIMULATION ========
  
  % Format Input Arguments
  isCellX = iscell(X);
  if ~isCellX, X = {X}; end;
  
  % Dimensions
  TS = size(X,2); % timesteps
  if ~isempty(X)
    Q = size(X{1},2); % samples/series
  else
    Q = 0;
  end
  
  % Allocate Outputs
  Y = cell(1,TS);
  
  % Time loop
  for ts=1:TS
  
    % Input 1
    Xp1 = mapminmax_apply(X{1,ts},x1_step1_gain,x1_step1_xoffset,x1_step1_ymin);
    
    % Layer 1
    a1 = tansig_apply(repmat(b1,1,Q) + IW1_1*Xp1);
    
    % Layer 2
    a2 = repmat(b2,1,Q) + LW2_1*a1;
    
    % Output 1
    Y{1,ts} = mapminmax_reverse(a2,y1_step1_gain,y1_step1_xoffset,y1_step1_ymin);
  end
  
  % Final Delay States
  Xf = cell(1,0);
  Af = cell(2,0);
  
  % Format Output Arguments
  if ~isCellX, Y = cell2mat(Y); end
end

% ===== MODULE FUNCTIONS ========

% Map Minimum and Maximum Input Processing Function
function y = mapminmax_apply(x,settings_gain,settings_xoffset,settings_ymin)
  y = bsxfun(@minus,x,settings_xoffset);
  y = bsxfun(@times,y,settings_gain);
  y = bsxfun(@plus,y,settings_ymin);
end

% Sigmoid Symmetric Transfer Function
function a = tansig_apply(n)
  a = 2 ./ (1 + exp(-2*n)) - 1;
end

% Map Minimum and Maximum Output Reverse-Processing Function
function x = mapminmax_reverse(y,settings_gain,settings_xoffset,settings_ymin)
  x = bsxfun(@minus,y,settings_ymin);
  x = bsxfun(@rdivide,x,settings_gain);
  x = bsxfun(@plus,x,settings_xoffset);
end