function dst = Poisson( src,Fh,Fv,msk)
%POISSON �˴���ʾ�йش˺�����ժҪ
%   ���ز��ɱ༭���

itr = 512;

K=[0,1,0;1,0,1;0,1,0];
p = ( msk > 0 );
lap = grad2lap(Fh,Fv);

dst = src;


for i = 1:itr
 lpf = imfilter(dst,K,'replicate');
 dst(p) = (lap(p) + lpf(p))/4;
 
end


function lap = grad2lap(Fh, Fv)
lap = circshift(Fh,[0,1]) + circshift(Fv,[1,0]) - Fh - Fv;
