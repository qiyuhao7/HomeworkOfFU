#include "HE_mesh/Mesh3D.h"
#include <Eigen/Dense>
#include <Eigen/Cholesky>


using namespace Eigen;
using namespace std;

class MinimalSurface
{
public:

	MinimalSurface(Mesh3D* meshIn);
	~MinimalSurface();

	Mesh3D* localWay();
	Mesh3D* globalWay();
	

private:
	Mesh3D *ptr_mesh_;
	Mesh3D *ptr_mesh_new;
	int vertex_list_length;

	
	vector<HE_vert*> verts;
	vector<HE_vert*> verts_new;

	void getRnew();
};
