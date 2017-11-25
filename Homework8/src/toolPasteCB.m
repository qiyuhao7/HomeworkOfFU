function toolPasteCB(varargin)

hpolys = evalin('base', 'hpolys');

roi = hpolys(1).getPosition(); %im2 �е����꣬����Ϊ����ԭ�� ÿ��Ϊһ����ĺ�������
targetPosition = roi + ceil(hpolys(2).getPosition() - roi); %im1 �е����� ͬ��

im1 = evalin('base', 'im1');
im2 = evalin('base', 'im2');
himg = evalin('base', 'himg');

imdst = blendImagePoisson(im1, im2, roi, targetPosition, hpolys);
set(himg, 'CData', imdst);
