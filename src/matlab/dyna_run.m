%cd '/Users/amorris/carma/projects/dynact'


mr_landmarks = csvread('/Users/amorris/carma/projects/dynact/AmidanBonnie_mr_cs_points.csv',1.0)

mr_x = mr_landmarks(:,2,:);
mr_y = mr_landmarks(:,3,:);
mr_z = mr_landmarks(:,4,:);
mr = [mr_x mr_y mr_z]

%R = rotz(90);
%mr = mr*R;


f1_landmarks = csvread('/Users/amorris/carma/projects/dynact/flouro/f1_crv.csv',1.0);
f1_x = f1_landmarks(:,2)- 147.84;
f1_y = f1_landmarks(:,3)- 147.84;
[N,~] = size(f1_x);
f1_z = zeros(N,1);
f1 = [f1_x f1_y]

f2_landmarks = csvread('/Users/amorris/carma/projects/dynact/flouro/f2_crv.csv',1.0);
f2_x = f2_landmarks(:,2)- 147.84;
f2_y = f2_landmarks(:,3)- 147.84;
[N,~] = size(f2_x);
f2_z = zeros(N,1);
f2 = [f2_x f2_y]



fp1 = '/Users/amorris/carma/projects/dynact/flouro/patient/fp1/IM-0001-0036.dcm';
fp2 = '/Users/amorris/carma/projects/dynact/flouro/patient/fp2/IM-0002-0036.dcm';

lmX1 = f1
lmX2 = f2
lmMR = mr


lmX1 = lmX1*[1 0; 0 -1;];
lmX2 = lmX2*[1 0; 0 -1;];



save('lmX1.mat','lmX1');
save('lmX2.mat','lmX2');
save('lmMR.mat','lmMR');
dlmwrite('lmX1.csv',lmX1);
dlmwrite('lmX2.csv',lmX2);
dlmwrite('lmMR.csv',lmMR);

[lmX,lmX13D,lmX23D,S,S1,S2,MX1,MX2,Mpro,ZMR] = SCIrunViz(fp1,fp2,lmX1,lmX2,lmMR) 

%lmX = lmX*[1 0 0; 0 0 1; 0 -1 0];

save('lmX.mat','lmX');
save('S.mat','S');
save('S1.mat','S1');
save('S2.mat','S2');
save('MX1.mat', 'MX1');
save('MX2.mat', 'MX2');
save('Mpro.mat','Mpro');
save('ZMR.mat', 'ZMR');

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


%dir
