function newv = local_way(v, f,is_boundary_vertices)
[nv,~] = size(v);

Mifs = indexedfaceset(v,f);
Mhe = halfedge(Mifs);

% compute cotanget Laplacan of mesh (x, t)
new1v = v;
new2v = v;
for i = 1:1000
    for j = 1:nv
        if is_boundary_vertices(j) == 1
            continue;
        else
            neighbors = Mhe.vertex_neighbors(j);
            sum = zeros(1,3);
            [~,nb] = size(neighbors);
            for k = 1:nb
                sum(1,:) = sum (1,:) + new1v(neighbors(k),:);
%                 sum(1,2) = sum (1,2) + new1v(neighbors(k),2);
%                 sum(1,3) = sum (1,3) + new1v(neighbors(k),3);
            end
            sum = sum / nb;
%             a = 0.7*new1v(j,:);
%             b = 0.3*sum;
            new2v(j,:) = 0.3*sum + 0.7*new1v(j,:);
        end
        
    end
    new1v = new2v;
end

newv = new1v;





