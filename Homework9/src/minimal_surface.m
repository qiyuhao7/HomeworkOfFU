[x, t] = readObj('../meshes/Cat_head.obj');
% x is n*3 3D coordinates of vertices
% t is n*3 indexes of vertices
%% draw 2 copies of the image
figure; set(gcf, 'Units', 'normalized', 'Position', [0.05,0.05,.8,.8]);
subplot(121); trimesh(t, x(:,1), x(:,2), x(:,3), 'edgecolor', 'k'); axis off; axis equal; title('input');
subplot(122); h = trimesh(t, x(:,1), x(:,2), x(:,3), 'edgecolor', 'k'); axis off; axis equal; title('output');

%% TODO: find boundary and interior vertices
is_boundary_vertices = is_boundary(x,t);
%% TODO: compute Laplacian
L = laplacian(x, t,is_boundary_vertices);
%% TODO: compute minimal surface using LOCAL approach
tic;
x = local_way(x,t,is_boundary_vertices);
toc;
%% TODO: compute minimal surface using GLOBAL approach
% tic;
% x = global_way(x,L,is_boundary_vertices); 
% toc;
%% draw mesh in minimal surface iterations
set(h, 'Vertices', x);