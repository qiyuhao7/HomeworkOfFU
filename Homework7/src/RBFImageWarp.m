function im2 = RBFImageWarp(im, psrc, pdst)
% input: im, psrc, pdst
% ax = zeros(100);
% ay = zeros(100);
ri2s = gitRi2s(psrc);
[ax,ay] = getAlpha(psrc,pdst,ri2s);
% basic image manipulations
% get image (matrix) size
[h, w, dim] = size(im);

im2 = im;

%set im2 white
for i=1:h
    for j=1:w
        for k=1:dim
            im2(i, j, k) = 255;
        end
    end
end

for i=1:h
    for j=1:w
        color = im(i,j,:);
        [dy,dx] = RBF(psrc,i,j,ax,ay,ri2s);
        
        dx = int32(dx);
        dy = int32(dy);
        if dx <= 0 || dx >w || dy <=0 ||dy >h
            continue;
        end
        im2(dx,dy,:) = color;
    end
end

% [n,~] = size(psrc);

% for i=1:n
%     for k=1:dim
%         im2(int32(psrc(i,1)), int32(psrc(i,2)), k) = 255;
%         im2(int32(pdst(i,1)), int32(pdst(i,2)), k) = 255;
%     end  
% end


end


%% TODO: compute warpped image
%功能为确定原顶点坐标对应的目标坐标
function [dx,dy] = RBF(psrc,sx,sy,ax,ay,ri2s)
    [n,~] = size(psrc);
    dx = 0;
    dy = 0;
    for i=1:n
        tmp_fi = distance2(psrc(i,1),psrc(i,2),sx,sy);
        tmp_fi = tmp_fi + ri2s(i);
        tmp_fi = sqrt(tmp_fi);
        dx = dx + ax(i,1)*tmp_fi;
        dy = dy + ay(i,1)*tmp_fi;
    end
    dx = dx + sx;
    dy = dy + sy;
end

%% 计算distance
function y = distance2(dx,dy,sx,sy)
    y = (dx-sx)*(dx-sx)+(dy-sy)*(dy-sy);
end

%% 计算alpha
function [ax,ay] = getAlpha(psrc,pdst,ri2s)
    [n,~] = size(psrc);
       
    A = zeros(n,n);
    for i = 1:n
        for j = 1:n
            tmp_fi = distance2(psrc(i,1),psrc(i,2),psrc(j,1),psrc(j,2));
            tmp_fi = tmp_fi + ri2s(i);
            tmp_fi = sqrt(tmp_fi) ;
            A(i,j) = tmp_fi;
        end
    end
    
    bx = zeros(n,1);
    by = zeros(n,1);
    for i = 1:n
        bx(i,1) = pdst(i,1) -  psrc(i,1);
        by(i,1) = pdst(i,2) -  psrc(i,2);
    end
    
    ax = A\bx;
    ay = A\by;
end
%%计算ri2s
function ri2s = gitRi2s(psrc)
    [n,~] = size(psrc);
    ri2s = zeros(1,n);
    for i = 1:n
       min_distance = 0;
       for j = 1:n
           disp(i)
           disp(j)
           if i==j 
               continue;
           else
               tmp_distance = distance2(psrc(i,1),psrc(i,2),psrc(j,1),psrc(j,2));
               if(min_distance > tmp_distance)
                   min_distance = tmp_distance;
               end
           end  %end if
       end %end for j
       ri2s(i) = 0;
    end %end for i
end

function [x,y] = IDW(psrc,pdst,sx,sy)
    [n,~] = size(psrc);
    sigmas = zeros(1,n);
    for i = 1:n
        distance = distance2(psrc(i,1),psrc(i,2),sx,sy);
        distance = sqrt(distance);
        sigma = 1.0/distance;
        sigmas(1,i) = sigma;
    end
    sigma_sum = 0;
    for i = 1:n
        sigma_sum = sigma_sum + sigmas(1,i);
    end
    
    weights = zeros(1,n);
    for i = 1:n
        weights(1,i) = sigmas(1,i)/sigma_sum;
    end
    
    fis = zeros(n,2);
    for i = 1:n
        tmp_psrc = pdst(i); 
        tmp_x = tmp_psrc(i,1) + sx - psrc(i,1);
        tmp_y = tmp_psrc(i,1) + sx - psrc(i,1);
        fis(i,1) = tmp_x;
        fis(i,2) = tmp_y;
    end
    
    x=0;
    y=0;
    for i = 1:n
        x = x + fis(i,1)*weights(1,i);
        y = y + fis(i,2)*weights(1,i);
    end
    
end







