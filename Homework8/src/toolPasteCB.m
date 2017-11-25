function toolPasteCB(varargin)

hpolys = evalin('base', 'hpolys');

roi = hpolys(1).getPosition(); %im2 中的坐标，左上为坐标原点 每行为一个点的横纵坐标
targetPosition = roi + ceil(hpolys(2).getPosition() - roi); %im1 中的坐标 同上

im1 = evalin('base', 'im1');
im2 = evalin('base', 'im2');
himg = evalin('base', 'himg');

imdst = blendImagePoisson(im1, im2, roi, targetPosition, hpolys);
set(himg, 'CData', imdst);
