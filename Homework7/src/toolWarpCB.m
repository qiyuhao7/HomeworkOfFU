function toolWarpCB(varargin)

hlines = evalin('base', 'hToolPoint.UserData');
im = evalin('base', 'im');
himg = evalin('base', 'himg');
%p2p 是一个二维的矩阵，每一行表示一个点的x,y坐标
%奇数行表示原顶点，偶数行表示目标顶点
p2p = zeros(numel(hlines)*2,2); 
for i=1:numel(hlines)
    p2p(i*2+(-1:0),:) = hlines(i).getPosition();
end

im2 = RBFImageWarp(im, p2p(1:2:end,:), p2p(2:2:end,:));
set(himg, 'CData', im2);