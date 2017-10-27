#include "MinimalSurface.h"

MinimalSurface::MinimalSurface(Mesh3D* meshIn)
{
	ptr_mesh_ = meshIn;
	
	vertex_list_length = ptr_mesh_->num_of_vertex_list();
	cout << vertex_list_length << endl;
}

MinimalSurface::~MinimalSurface()
{

}

Mesh3D* MinimalSurface::localWay()
{
	//cout << "local way" << endl;
	vector<HE_vert*>* verts_pointer = ptr_mesh_->get_vertex_list();
	verts = *(ptr_mesh_->get_vertex_list());
	verts_new = verts;

	for (size_t i = 0; i < 2000; i++)
	{
		getRnew();
	}

	*verts_pointer = verts_new;
	ptr_mesh_->UpdateNormal();
	return ptr_mesh_;
}

void MinimalSurface::getRnew()
{
	
	for (size_t i = 0; i < vertex_list_length; i++)
	{
		if (verts[i]->isOnBoundary())
		{
			continue;
		}
		else
		{
			
			HE_edge* edge = verts[i]->pedge_;
			Vec3f sum(0.0, 0.0, 0.0);
			Vec3f& r = verts[i]->position();

			int num_neighbor = 0;
			do{
				
				
				num_neighbor++;
				Vec3f tmp = edge->pvert_->position();
				
				sum += edge->pvert_->position();
				
				
				edge = edge->ppair_;
				edge = edge->pnext_;
			} while (edge != verts[i]->pedge_);

			

			Vec3f p = Vec3f(num_neighbor*r.x() - sum.x(), num_neighbor*r.y() - sum.y(), num_neighbor*r.z() - sum.z());

			Vec3f t = Vec3f(p.x() / num_neighbor, p.y() / num_neighbor, p.z() / num_neighbor);

			Vec3f rnew = r - Vec3f(t.x() / 3, t.y() / 3, t.z() / 3);

			verts_new[i]->set_position(rnew);

		}
	}
	
	verts = verts_new;
}



Mesh3D* MinimalSurface::globalWay()
{
	vector<HE_vert*>* verts_pointer = ptr_mesh_->get_vertex_list();
	verts = *(ptr_mesh_->get_vertex_list());

	int& m = vertex_list_length;

	//Step 1
	MatrixXd A(m, m);
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			A(i, j) = 0;
		}
	}

	for (size_t i = 0; i < m; i++)
	{
		//cout << verts[i]->id() << endl;
		if (verts[i]->isOnBoundary())
		{
			A(i, i) = 1;
		}
		else
		{
			int num_neighbor = 0;
			HE_edge* edge = verts[i]->pedge_;
			do{
				int ki = edge->pvert_->id();

				A(i, ki) = -1;

				num_neighbor++;
				edge = edge->ppair_;
				edge = edge->pnext_;
			} while (edge != verts[i]->pedge_);

			A(i, i) = num_neighbor;
		}

	}


	//Step 2
	VectorXd ConX(m);
	VectorXd ConY(m);
	VectorXd ConZ(m);

	for (size_t i = 0; i < m; i++)
	{
		ConX(i) = 0;
		ConY(i) = 0;
		ConZ(i) = 0;
	}

	for (size_t i = 0; i < m; i++)
	{
		Vec3f& r = verts[i]->position();
		if (verts[i]->isOnBoundary())
		{
			ConX(i) = r.x();
			ConY(i) = r.y();
			ConZ(i) = r.z();
		}
		else
		{
			ConX(i) = 0;
			ConY(i) = 0;
			ConZ(i) = 0;
		}
	}

	//Step 3
	VectorXd X(m);
	VectorXd Y(m);
	VectorXd Z(m);

	X = A.householderQr().solve(ConX);
	Y = A.householderQr().solve(ConY);
	Z = A.householderQr().solve(ConZ);

	//Step 4
	for (size_t i = 0; i < m; i++)
	{
		Vec3f& r = verts[i]->position();
		r.x() = X(i);
		r.y() = Y(i);
		r.z() = Z(i);
	}


	*verts_pointer = verts;
	ptr_mesh_->UpdateNormal();
	return ptr_mesh_;
}