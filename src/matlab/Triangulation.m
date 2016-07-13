function [lmX3D] = Triangulation(lmX1,lmX2, K1, R1, t1, K2, R2, t2, lmX13D, lmX23D)
% [lmX3D,W] = Triangulation(lmX1,lmX2, K1, R1, t1, K2, R2, t2) performs 
% triangulation from two sets of fluorscopy landmarks, acquired at two
% different angles. The algorithm is based on the work of Brost et al. 2009
%
% Input: 
%       - lmX1: landmark set belonging to fluoroscopy image 1
%       - lmX2: landmark set belonging to fluoroscopy image 2
%       - K1: intrinsic matrix of C-arm system of fluoroscopy image 1
%       - R1: rotation matrix of C-arm system of fluoroscopy image 1
%       - t1: translation vector of C-arm system of fluoroscopy image 1
%       - K2: intrinsic matrix of C-arm system of fluoroscopy image 2
%       - R2: rotation matrix of C-arm system of fluoroscopy image 2
%       - t2: translation vector of C-arm system of fluoroscopy image 2
% 
% Output:
%       - lmX3D: the triangulated landmarks given in the world coordinate
%       system
% 
% Author:
%       Mette Lindegaard, August 2013
%       The Technical University of Denmark & The University of Utah

W=[];
[N,~] = size(lmX1);

% Source positions:
%o1 = -inv(R1)*t1;
%o2 = -inv(R2)*t2;

o1 = t2;
o2 = t1;

b = o2-o1;

% Changing to homogeneous coordinates
%v1 = [lmX1 ones(N,1)];
%v2 = [lmX2 ones(N,1)];

for k=1:N

    % Direction of lines
    %d1 = inv(R1)*inv(K1)*v1(k,:)';
    %d2 = inv(R2)*inv(K2)*v2(k,:)';
    
    d1 = o1 - lmX13D(k,:)';
    d2 = o2 - lmX23D(k,:)';

    % normalize
    d1 = d1./sum(d1);
    d2 = d2./sum(d2);

    C = [d1 d2];

    a = (pinv(C)*b); %pseudo inverse
    nu = a(1);
    mu = -a(2);

    w = 1/2*(o1+nu*d1+o2+mu*d2);
    W = [W ;w'];

end

lmX3D = W; % resulting 3D landmarks


