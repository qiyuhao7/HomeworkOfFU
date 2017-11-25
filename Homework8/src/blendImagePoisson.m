function imret = blendImagePoisson(im1, im2, roi, targetPosition, hpolys)
%关于double和uint的一些解释
% MATLAB looks at the datatype to decide which range of values to expect. uint8 are expected to be 0 to 255. double are expected to be 0 to 1. When you double() a uint8 you end up with 0. to 255. and everyt value from 1 upwards will be considered to saturate the maximum 9 to 1 range.


MaskTarget = createMask(hpolys(2));
%MaskSource = createMask(hpolys(1));

[h2,w2,~] = size(im2);
%[h1,w1,~] = size(im1);
[n,~] = size(roi);
roi = int32(roi);
targetPosition = int32(targetPosition);
%right polygon's bias
biasX = int32(targetPosition(1,1)- roi(1,1));
biasY = int32(targetPosition(1,2)- roi(1,2));
%find coordinate of polygon of im2
h2Min = h2; 
w2Min = w2;
h2Max = 1;
w2Max = 1;
for i=1:n
    if roi(i,2) < h2Min
        h2Min = roi(i,2);
    end
    if roi(i,2) > h2Max
        h2Max = roi(i,2);
    end
end

for i=1:n
    if roi(i,1) < w2Min
        w2Min = roi(i,1);
    end
    if roi(i,1) > w2Max
        w2Max = roi(i,1);
    end
end


%S = sea， B = bear
%h = horizon , v = vertical
%f = forward
%必须要转换为double
% im1d = double(im1); 
% im2d = double(im2);
% [Sh,Sv] = imgrad(im1d);
% [Bh,Bv] = imgrad(im2d);
% 
% X = im1d; %copy of im1
% Fh = Sh;
% Fv = Sv;

% X = immask(im2,X,MaskTarget,biasX,biasY,h2Min,h2Max,w2Min,w2Max);
% Fh = immask(Bh,Fh,MaskTarget,biasX,biasY,h2Min,h2Max,w2Min,w2Max);
% Fv = immask(Bv,Fv,MaskTarget,biasX,biasY,h2Min,h2Max,w2Min,w2Max);

% 
% maskTarget3D = zeros(size(X));
% maskTarget3D(:,:,1) = MaskTarget;
% maskTarget3D(:,:,2) = MaskTarget;
% maskTarget3D(:,:,3) = MaskTarget;
%裁剪
% Xc = X(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:);
% Fhc = Fh(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:);
% Fvc = Fv(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:);
im1c = im1(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:);
MaskTargetc = MaskTarget(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:);
im2c = im2(h2Min:h2Max,w2Min:w2Max,:);

% [r,c] = find(MaskTargetc == 1);
% maxW = w2Max - w2Min;
% maxH = h2Max - h2Min; 
% im1cc = imcrop(im1c,[min(c) min(r) maxW maxH]);
% MaskTargetcc = imcrop(MaskTargetc,[min(c) min(r) maxW maxH]);
% im2cc = imcrop(im1c,[min(c) min(r) maxW maxH]);
% 
% K=[0,1,0;1,0,1;0,1,0];
% p = ( msk > 0 );
%lap = circshift(Fhc,[0,1]) + circshift(Fvc,[1,0]) - Fhc - Fvc;
% lpf = imfilter(im1c,K,'replicate');

laplacian_mask=[0 1 0; 1 -4 1; 0 1 0];
MaskTargetc(1,:)=0;
MaskTargetc(end,:)=0;
MaskTargetc(:,1)=0;
MaskTargetc(:,end)=0;


n=size(find(MaskTargetc==1),1);

map=zeros(size(MaskTargetc));


counter=0;
for x=1:size(map,1)
    for y=1:size(map,2)
        if MaskTargetc(x,y)==1;
            counter=counter+1;
            map(x,y)=counter;  
            %in the 1D vector
        end
    end
end
%%算A

im_out = im1c;
for i=1:3
    A=spalloc(n,n,n*5);
    B=zeros(n,1);
    lap=conv2(im2c(:,:,i),laplacian_mask, 'same');
    counter=0;
    for x=1:size(map,1)
        for y=1:size(map,2)
            if MaskTargetc(x,y)==1
                counter=counter+1;
                A(counter,counter)=4; 
                
                if MaskTargetc(x-1,y)==0 
                    B(counter)=im1c(x-1,y,i); 
                else 
                    A(counter,map(x-1,y))=-1; 
                end
                if MaskTargetc(x+1,y)==0 
                    B(counter)=B(counter)+im1c(x+1,y,i); 
                else 
                    A(counter,map(x+1,y))=-1;
                end
                if MaskTargetc(x,y-1)==0 
                    B(counter)=B(counter)+im1c(x,y-1,i); 
                else 
                    A(counter,map(x,y-1))=-1; 
                end
                if MaskTargetc(x,y+1)==0 
                    B(counter)=B(counter)+im1c(x,y+1,i);
                else 
                    A(counter,map(x,y+1))=-1; 
                end
                
                
                B(counter)=B(counter)-lap(x,y);
                
            end
        end
    end
    tic;
    %LU分解
    [L,U] = lu(A);
    y = L\B;
    Xr = U\y;
    toc;
    
    tic;
    %cholesky分解
    L = chol(A,'lower');
    R = chol(A,'upper');
    y = L\B;
    Xr = R\y;
    toc;
    
    tic;
    %QR分解
    [C,R,P] = qr(A,B);
    Xr = P*(R\C);
    toc;
    
    tic;
    %普通求解
    Xr=A\B;
    toc;
    for counter=1:length(Xr)
        [index_x,index_y]=find(map==counter);
        im_out(index_x,index_y,i)=Xr(counter);
        
    end
end

Y = im1;
Y(biasY+h2Min:biasY+h2Max,biasX+w2Min:biasX+w2Max,:) = im_out;
%Y = Poisson( X, Fh, Fv, maskTarget3D ,A);
%必须转化为uint 8
imret = uint8(Y);

