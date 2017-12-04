function is_boundary_vertices = is_boundary(v,f)
%IS_BOUNDARY �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
[nv,~] = size(v);
[nf,~] = size(f);
%MCGL �⹹��������ݽṹ

%���� �Ƿ�Ϊ�߽�
is_boundary_matrix = zeros(nv,nv);
is_boundary_matrix = is_boundary_matrix - ones(nv,nv);

for i = 1:nf
    is_boundary_matrix(f(i,1),f(i,2)) =is_boundary_matrix(f(i,1),f(i,2))+1;
    is_boundary_matrix(f(i,2),f(i,1)) =is_boundary_matrix(f(i,2),f(i,1))+1;
    is_boundary_matrix(f(i,3),f(i,1)) =is_boundary_matrix(f(i,3),f(i,1))+1;
    is_boundary_matrix(f(i,1),f(i,3)) =is_boundary_matrix(f(i,1),f(i,3))+1;
    is_boundary_matrix(f(i,2),f(i,3)) =is_boundary_matrix(f(i,2),f(i,3))+1;
    is_boundary_matrix(f(i,3),f(i,2)) =is_boundary_matrix(f(i,3),f(i,2))+1;
end

is_boundary_vertices = zeros(1,nv);
for i = 1:nv
    for j =i:nv
        if is_boundary_matrix(i,j) ==0
            is_boundary_vertices(i) =1;
            is_boundary_vertices(j) =1;
        end
    end
end

end

