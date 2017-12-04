function L = laplacian(v, f,is_boundary_vertices)

% compute cotanget Laplacan of mesh (x, t)
[nv,~] = size(v);

%MCGL 库构建半边数据结构
Mifs = indexedfaceset(v,f);
Mhe = halfedge(Mifs);

L = zeros(nv, nv);

for i = 1:nv
    if is_boundary_vertices(i) == 1
        L(i,i) = 1;
    else
        neighbors = Mhe.vertex_neighbors(i);
        [~,nb] = size(neighbors);
        for k = 1:nb
            L(i,neighbors(k)) = -1;
        end
        L(i,i) = nb;
    end
end

L = sparse(L);

