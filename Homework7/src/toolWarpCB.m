function toolWarpCB(varargin)

hlines = evalin('base', 'hToolPoint.UserData');
im = evalin('base', 'im');
himg = evalin('base', 'himg');
%p2p ��һ����ά�ľ���ÿһ�б�ʾһ�����x,y����
%�����б�ʾԭ���㣬ż���б�ʾĿ�궥��
p2p = zeros(numel(hlines)*2,2); 
for i=1:numel(hlines)
    p2p(i*2+(-1:0),:) = hlines(i).getPosition();
end

im2 = RBFImageWarp(im, p2p(1:2:end,:), p2p(2:2:end,:));
set(himg, 'CData', im2);