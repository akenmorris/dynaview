%cd '/Users/amorris/carma/projects/dynact'


mr_landmarks = csvread('mri.csv',1.0)

mr_x = mr_landmarks(:,2,:);
mr_y = mr_landmarks(:,3,:);
mr_z = mr_landmarks(:,4,:);
mr = [mr_x mr_y mr_z]

%R = rotz(90);
%mr = mr*R;


fp1 = 'f1.dcm';
fp2 = 'f2.dcm';


[SID,SOD,p,u0,v0] = Xinfo(fp1);

f1_landmarks = csvread('f1.csv',1.0);

%p=0

f1_x = f1_landmarks(:,2) - (u0*p);
f1_y = f1_landmarks(:,3) - (v0*p);
[N,~] = size(f1_x);
f1_z = zeros(N,1);
f1 = [f1_x f1_y]

[SID,SOD,p,u0,v0] = Xinfo(fp2);

f2_landmarks = csvread('f2.csv',1.0);
f2_x = f2_landmarks(:,2) - (u0*p);
f2_y = f2_landmarks(:,3) - (v0*p);
[N,~] = size(f2_x);
f2_z = zeros(N,1);
f2 = [f2_x f2_y]



lmX1 = f1
lmX2 = f2
lmMR = mr


lmX1 = lmX1*[1 0; 0 -1;];
lmX2 = lmX2*[1 0; 0 -1;];



dlmwrite('lmX1.csv',lmX1);
dlmwrite('lmX2.csv',lmX2);
dlmwrite('lmMR.csv',lmMR);

[lmX,lmX13D,lmX23D,S,S1,S2,MX1,MX2,Mpro,ZMR] = SCIrunViz(fp1,fp2,lmX1,lmX2,lmMR) 

dlmwrite('lmX.csv',lmX);
dlmwrite('lmX13D.csv',lmX13D);
dlmwrite('lmX23D.csv',lmX23D);
dlmwrite('S.csv',S);
dlmwrite('S1.csv',S1);
dlmwrite('S2.csv',S2);
dlmwrite('MX1.csv', MX1);
dlmwrite('MX2.csv', MX2);
dlmwrite('Mpro.csv',Mpro);
dlmwrite('ZMR.csv', ZMR);
