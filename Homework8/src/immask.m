function result = immask( src,dst,mask,biasX,biasY,hsMin,hsMax,wsMin,wsMax)
%IMMASK 把src 根据 mask 放置到 dst 上

src2dst = dst;
src2dst(biasY+hsMin:biasY+hsMax,biasX+wsMin:biasX+wsMax,:) = src(hsMin:hsMax,wsMin:wsMax,:);

src2dst(:, :, 1) = double(src2dst(:, :, 1)).*mask;  
src2dst(:, :, 2) = double(src2dst(:, :, 2)).*mask;
src2dst(:, :, 3) = double(src2dst(:, :, 3)).*mask;

result = dst;
oneMinusMask = 1 - mask;
result(:, :, 1) = double(result(:, :, 1)).*oneMinusMask;
result(:, :, 2) = double(result(:, :, 2)).*oneMinusMask;
result(:, :, 3) = double(result(:, :, 3)).*oneMinusMask;

result(biasY+hsMin:biasY+hsMax,biasX+wsMin:biasX+wsMax,:) = result(biasY+hsMin:biasY+hsMax,biasX+wsMin:biasX+wsMax,:) + src2dst(biasY+hsMin:biasY+hsMax,biasX+wsMin:biasX+wsMax,:);

end

