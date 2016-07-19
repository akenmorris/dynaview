function [SID SOD p u0 v0] = Xinfo(imagepath)
% This function creates the projection matrix from dicom header info of
% fluoroscopy images

info = dicominfo(imagepath);

SID = info.DistanceSourceToDetector;


if isfield(info,{'DistanceSourceToPatient'}) == 1; 
    SOD = info.DistanceSourceToPatient; 
else
    % SOD = SID/2+20; % assumption when DistanceSourceToPatient is missing
    SOD = 785; % According to DynaCT calibration file (g_Robot_TL15_Left_480_L_A_1.50_2x2)
end

SID=1024
%SOD=785

nu = double(info.Width);
nv = double(info.Height);


if isfield(info,{'DistanceSourceToPatient'}) == 1
    p = info.ImagerPixelSpacing(1);
else
    p = 0.308;
end

u0 = nu/2; % image center x
v0 = nv/2; % image center y


end