function [K R t P iloc rloc] = PMatrix(imagepath)
% [K R t P] = PMatrix(imagepath) calculates the projection matrix from the
% dicom header parameters of fluoroscopy images
%
% Input: 
%       - imagepath: the full path of the fluoroscopy image file
% 
% Output:
%       - K: A 3x3 matrix containing the intrinsic parameters of the C-arm
%       - R: A 3x3 rotation matrix
%       - t: A 1x3 translation vector
%       - P: The projection matrix calculated from P=K*[R t]
%       - iloc: The intensifier location
%       - rloc:
% Author:
%       Mette Lindegaard, August 2013
%       The Technical University of Denmark & The University of Utah

% Extracting the dicom header information
info = dicominfo(imagepath);

IS = info.IntensifierSize;
PA = info.PositionerPrimaryAngle; % Primary angle
PA = PA/180*pi; % radians
SA = info.PositionerSecondaryAngle; % Secondary angle
SA = SA/180*pi; % radians
SID = info.DistanceSourceToDetector;

if isfield(info,{'DistanceSourceToPatient'}) == 1; 
SOD = info.DistanceSourceToPatient; 
else
SOD = SID/2+20; % assumption when DistanceSourceToPatient is missing
end

% Number of pixels
nu = double(info.Width);
nv = double(info.Height);

if isfield(info,{'ImagerPixelSpacing'}) == 1; 
p = info.ImagerPixelSpacing(1);
if p==0
    p=0.308; % assumption when DistanceSourceToPatient is zero
end
else
    p=0.308; % assumption when DistanceSourceToPatient is missing
end

u0 = nu/2*p; % image center u
v0 = nv/2*p; % image center v

u0=0; %akm
v0=0; %akm

%K = [SID/p 0 u0; 0 SID/p v0; 0 0 1]*[1 0 0; 0 0 1; 0 1 0];
%K = [SID 0 u0; 0 SID v0; 0 0 1]*[1 0 0; 0 0 1; 0 1 0]; %akm
K = [SID 0 u0; 0 SID v0; 0 0 1]; %akm
K

t = [0 SOD 0]';



Rx = rotx(90);
Rpa = [cos(PA) -sin(PA) 0; sin(PA) cos(PA) 0 ; 0 0 1]; % primary rotation
Rsa = [1 0 0; 0 cos(SA) -sin(SA); 0 sin(SA) cos(SA)]; % secondary rotation
R = Rpa*Rsa*Rx;

RR = Rpa*Rsa;

t = Rpa*Rsa*t;

%R = Rx*Rpa*Rsa;
%R = Rpa*Rx*Rsa;
    
P = K*[R t]; % The projection matrix
P


iloc=[0 -(SID-SOD) 0]*RR; %akm
rloc=RR'*Rx;

end