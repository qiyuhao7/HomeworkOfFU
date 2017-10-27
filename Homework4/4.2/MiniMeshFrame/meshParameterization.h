#include <Eigen/Dense>
#include <Eigen/Cholesky>

#include "HE_mesh/Vec.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

using trimesh::vec;
using trimesh::point;
typedef trimesh::point point;
typedef trimesh::vec3  Vec3f;
typedef trimesh::vec4  Vec4f;

using namespace Eigen;
using namespace std;


typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

class MeshParameterization
{
public:
	MeshParameterization(MyMesh meshIn);
	~MeshParameterization();

	MyMesh uniform();
	MyMesh weightedLeastSquares();
	MyMesh shapePreserving();

	MyMesh mesh;

private:

	int N;
	int n;
	
	MyMesh meshResult;

};