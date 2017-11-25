function im2 = IDWImageWarp(im, psrc, pdst)
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
        [dx,dy] = IDW(psrc,pdst,j,i);
        
        dx = int32(dx);
        dy = int32(dy);
        if dx <= 0 || dx >w || dy <=0 ||dy >h
            continue;
        end
        im2(dy,dx,:) = color;
    end
end

end

function [x,y] = IDW(psrc,pdst,sx,sy)
    [n,~] = size(psrc);
    sigmas = zeros(1,n);
    for i = 1:n
        tmp_distance = distance2(psrc(i,1),psrc(i,2),sx,sy);
        tmp_distance = sqrt(tmp_distance);
        sigma = 1.0/tmp_distance;
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
        tmp_pdst = pdst; 
        tmp_x = tmp_pdst(i,1) + sx - psrc(i,1);
        tmp_y = tmp_pdst(i,2) + sy - psrc(i,2);
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

function y = distance2(dx,dy,sx,sy)
    y = (dx-sx)*(dx-sx)+(dy-sy)*(dy-sy);
end

