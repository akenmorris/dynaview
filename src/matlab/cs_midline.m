% This script is used to compute the midline, based on the segmentation
% of the coronary sinus imported from Corview (Volumetric Image
% Segmentation and Visualization. Comprehensive Arrhythmia Research and
% Management Center (CARMA), downloaded from internal server).
% Required input:
%  - Coronary sinus segmentation mask imported from Corview
%  - voxel scaling factors, imported from the dicom header of the LGE−MRI data file.
% Calculated output:
%  − 3D Midline of the coronary sinus in real world distances
%Author:
%      Mette Lindegaard, August 2013
%      The Technical University of Denmark & The University of Utah
clear all; close all
% Loading segmedted mask files exported from Corview in .mat format
maskpath0 = ['/Users/mettelindegaard/Patient Example 2/MRI Masks/',...
'CSsegmentation/MaskLayerPE2.mat'];
maskpath1 = ['/Users/mettelindegaard/Anonymized old data/Patient1/',...
'MaskLayerP1.mat'];
maskpath2 = ['/Users/mettelindegaard/Anonymized old data/Patient2/',...
'MaskLayerP2.mat'];
maskpath5 = ['/Users/mettelindegaard/Anonymized old data/Patient5/',...
'MaskLayerP5.mat'];
maskpath6 = ['/Users/mettelindegaard/Anonymized old data/Patient6/',...
'MaskLayerP6.mat'];
maskpaths = {maskpath0,maskpath1,maskpath2,maskpath5,maskpath6};

figure
for m = 3:length(maskpaths)
load(maskpaths{m})
mask = scirunnrrd.data;

 % finding mask (values = 1)
 [u,v,s] = ind2sub(size(mask),find(mask));
 N = length(u); % # volume pixels

 us = unique(s); % LGE−MRI slices containing mask
￼M=[];
 for i = 1:length(us) % calculating midline for each LGE−MRI slice find(s==us(i));
   a = find(s==us(i));
   [M; mean(u(a)) mean(v(a)) us(i)]; % 3D midline coordinates
 end

% LGE−MRI scaling information:
Minfo = dicominfo(['/Users/mettelindegaard/Patient Example 2/',...
'MRI Masks/Original/Original−00']);

scaling = [Minfo.PixelSpacing; Minfo.SliceThickness]';
Mscaled = M.*repmat(scaling,length(M),1);

subplot(1,3,m−2)
plot3(Mscaled(:,2),Mscaled(:,1),Mscaled(:,3)) % midline MRI points
grid on
axis image
xlabel('x [mm]'); ylabel('y [mm]'); zlabel('z [mm]');
end