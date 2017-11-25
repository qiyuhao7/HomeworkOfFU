function dst = Poisson( src,Fh,Fv,msk)
%POISSON 此处显示有关此函数的摘要
%   返回泊松编辑结果

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
