function newv = global_way(v,L,is_boundary_vertices)
%GLOBAL_WAY �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
[nv,~] = size(v);
B = zeros(nv,3);

for i = 1:nv
    if is_boundary_vertices(i) == 1
        B(i,:) = v(i,:);
    end
end

newv = L \ B;
end

