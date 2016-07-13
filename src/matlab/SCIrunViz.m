function [lmX,S,S1,S2,MX1,MX2,Mpro,ZMR] = SCIrunViz(fp1,fp2,lmX1,lmX2,lmMR)

% [lmX,S,S1,S2,MX1,MX2,Mpro,ZMR] = SCIrunViz(fp1,fp2,lmX1,lmX2,lmMR) 
% prepares the output of the registration to comply with the formats in 
% SCIRun (A Scientific Computing Problem Solving Environment, Scientific 
% Computing and Imaging Institute (SCI), Download from: 
% http://www.scirun.org.) 
%
% Input: 
%       - fp1: full path of fluoroscopy image 1
%       - fp2: full path of fluoroscopy image 2
%       - lmX1: landmark set of fluoroscopy image 1
%       - lmX2: landmark set of fluoroscopy image 2
%       - lmMR: landmarks of DE-MRI volume
% 
% Output:
%       - lmX: the triangulated landmarks in the world coordinate system
%       - S: C-arm source positions
%       - S1: C-arm source positions for fluroscopy image 1
%       - S2: C-arm source positions for fluroscopy image 2
%       - MX1: alignment matrix for fluoroscopy image 1
%       - MX2: alignment matrix for fluoroscopy image 2
%       - Mpro: procrustes alignment matrix from registration result
%       - ZMR: MR landmark after procrustes transformation
% 
% Author:
%       Mette Lindegaard, August 2013
%       The Technical University of Denmark & The University of Utah

[K1, R1, t1, P1, iloc1, rloc1] = PMatrix(fp1); % projection matrix 1
[K2, R2, t2, P2, iloc2, rloc2] = PMatrix(fp2); % projection matrix 2

MX1 = [rloc1 iloc1'];
MX2 = [rloc2 iloc2'];


% Aligning fluroscopy landmarks:
nlm = length(lmX1);
lmX13D = [lmX1 zeros(nlm,1) ones(nlm,1)]*MX1';
lmX23D = [lmX2 zeros(nlm,1) ones(nlm,1)]*MX2';

% Triangulating fluroscopy landmarks:
[lmX3D] = Triangulation(lmX1, lmX2, K1, R1, t1, K2, R2, t2, lmX13D, lmX23D);

%%%%%%%%%%%%%%%%tmp
%lmX3D
%lmX3D = lmX3D*[1 0 0; 0 -1 0; 0 0 1];
%lmX3D

%return

% C-arm source positions:

S1 = t1;
S2 = t2;


S = [S1 S2]';

          

% Procrustes alignment:
[dMR ZMR TMR] = procrustes(lmX3D,lmMR,'reflection',false,'scaling',false)

Mpro = [TMR.T*TMR.b; TMR.c(1,:)]'; % Procrustes transformation matrix





%lmX23D = (MX2*[lmX2 zeros(nlm,1) ones(nlm,1)]')';

%lmX23D = (rotz(-30)*[lmX2 zeros(nlm,1)]')';


%lmX23D = [lmX2 zeros(nlm,1) ones(nlm,1)]*MX2';

% Preparing output for SCIrun
lmX = [lmX13D;lmX23D;lmX3D]
%lmX = [lmX23D];
%lmX = [lmX3D]
%%%lmX = struct('x',lmX(:,1),'y',lmX(:,2),'z',lmX(:,3))
%%%ZMR = struct('x',ZMR(:,1),'y',ZMR(:,2),'z',ZMR(:,3))
MX1 = [MX1; 0 0 0 1]
MX2 = [MX2; 0 0 0 1]
Mpro = [Mpro; 0 0 0 1]

%S1 = S1'*[1 0 0; 0 0 1; 0 -1 0] % for SCIRun matrix output
%S2 = S2'*[1 0 0; 0 0 1; 0 -1 0]
%S1 = S1'*[1 0 0; 0 0 1; 0 -1 0] % for SCIRun matrix output
%S2 = S2'*[1 0 0; 0 0 1; 0 -1 0]
end

