function [Fh,Fv] = imgrad(X)

Kh = [ 0,-1, 1 ]; %行卷积核
Kv = [ 0;-1; 1 ]; %列卷积核

Fh = imfilter(X,Kh,'replicate'); 
Fv = imfilter(X,Kv,'replicate');

end

