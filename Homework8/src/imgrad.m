function [Fh,Fv] = imgrad(X)

Kh = [ 0,-1, 1 ]; %�о����
Kv = [ 0;-1; 1 ]; %�о����

Fh = imfilter(X,Kh,'replicate'); 
Fv = imfilter(X,Kv,'replicate');

end

