#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#define FREEGLUT_STATIC
#include "gl_core_3_3.h"
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#define TW_STATIC
#include <AntTweakBar.h>

#include <ctime>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <Eigen/Dense> 
#include <time.h>
#include "objloader.h"
#include "glprogram.h"
#include "MyImage.h"
#include "VAOImage.h"
#include "VAOMesh.h"
#include "trackball.h"
#include "math.h"
#include "matlab_utils.h"

#include <algorithm>


GLProgram MyMesh::prog, MyMesh::pickProg, MyMesh::pointSetProg;
GLTexture MyMesh::colormapTex;

MyMesh M;
int viewport[4] = { 0, 0, 1280, 960 };
bool showATB = true;

using MatX3f = Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>;
using MatX3i = Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>;
MatX3f meshX;//vertices
MatX3i meshF;//faces
int nEigs = 1000;
int m = 50;



struct node
{
	double value;
	int index;
};

Eigen::MatrixXf Laplacian(MatX3f &meshX, MatX3i &meshF)
{
	using namespace Eigen;
	int N = meshX.size() / 3;// numVertex
	int F = meshF.size() / 3;//numFaces
	//SparseMatrix<float> L(N, N);
	MatrixXf L = MatrixXf::Zero(N, N); //Laplacian L
	MatrixXf A = MatrixXf::Zero(N, N);//Adjacency A

	//set Adjacent Matrix A
	for (size_t j = 0; j < F; j++)
	{
		//vertices of the face j 
		int v1 = meshF(j, 0);
		int v2 = meshF(j, 1);
		int v3 = meshF(j, 2);

		//set Adjacent Matrix
		A(v1, v2) = 1;
		A(v2, v1) = 1;
		A(v1, v3) = 1;
		A(v3, v1) = 1;
		A(v2, v3) = 1;
		A(v3, v2) = 1;
	}

	//set Laplacion Matrix L
	for (size_t i = 0; i < N; i++)
	{
		L(i, i) = 1;
		int d = 0;//num of adjacent vertices
		for (size_t j = 0; j < N; j++)
		{
			if (A(i, j) == 1)
			{
				d++;
			}
		}
		for (size_t j = 0; j < N; j++)
		{
			if (A(i, j) == 1)
			{
				L(i, j) = -1.0 / d;
			}
		}
	}

	return L;
}

extern "C"
float* mulWithCUDA(const float *A, const float *B, const float *C, const int m, const int n, const int e);

void mulEigenWithCUDA(Eigen::MatrixXf &MA, Eigen::MatrixXf &MB, Eigen::MatrixXf &MC)
{

	float* A = MA.data();
	float* B = MB.data();
	float* C = MC.data();
	//注意！！！！ 这个M.data()是以列开始排列的，这里结算的是C的转置 = B转置*A转置
	mulWithCUDA(B, A, C, MB.cols(), MA.rows(), MA.cols());

}

void SmoothCUDA(int n)
{
	if (n < 0) {
		M.upload(meshX.data(), meshX.rows(), nullptr, 0, nullptr);//upload 应该是上传图像以显示
		return;
	}

	using namespace Eigen;
	using namespace std;
	std::cout << meshX.rows() << endl;

	// TODO, compute laplacian, and sent to Matlab
	int N = meshX.size() / 3;


	MatrixXf L = Laplacian(meshX, meshF); //注意L是一个稀疏矩阵

	double start, end, cost;
	start = clock();
	MatrixXf I = MatrixXf::Identity(N, N);
	MatrixXf S = I - L*(1.0 / 2.0);
	MatrixXf x2 = meshX;

	for (size_t i = 0; i < m; i++)
	{
		MatrixXf T(N,3);
		mulEigenWithCUDA(S,x2,T);
		x2 = T;
	}
	end = clock();
	cost = end - start;
	cout << cost << endl;

	Matrix<float, Dynamic, Dynamic, RowMajor> y;// y是n*3的矩阵,
	y = x2;

	if (y.cols() > 3)  y = y.leftCols(3);
	if (y.rows() == 0 || y.cols() != 3) return;

	M.upload(y.data(), y.rows(), nullptr, 0, nullptr); //显示变换后的y

													   // TODO 3: compute reconstruction error
	MatX3f meshErro;
	vector<float> distance;
	for (size_t i = 0; i < N; i++)
	{
		double tmp = 0;
		tmp += ((y(i, 0) - meshX(i, 0)) * (y(i, 0) - meshX(i, 0)));
		tmp += ((y(i, 1) - meshX(i, 1)) * (y(i, 1) - meshX(i, 1)));
		tmp += ((y(i, 2) - meshX(i, 2)) * (y(i, 2) - meshX(i, 2)));
		distance.push_back(sqrt(tmp));
	}

	std::vector<float> err(meshX.rows(), 0);
	M.setVertexDataViz(distance.data());

}

void SmoothEigen(int n)
{
	if (n < 0) {
		M.upload(meshX.data(), meshX.rows(), nullptr, 0, nullptr);//upload 应该是上传图像以显示
		return;
	}

	using namespace Eigen;
	using namespace std;
	std::cout << meshX.rows() << endl;

	// TODO, compute laplacian, and sent to Matlab
	int N = meshX.size() / 3;

	
	MatrixXf L = Laplacian(meshX, meshF); //注意L是一个稀疏矩阵

	double start, end, cost;
	start = clock();
	MatrixXf I = MatrixXf::Identity(N, N);
	MatrixXf S = I - L*(1.0 / 2.0);
	MatrixXf x2 = meshX;
	for (size_t i = 0; i < m; i++)
	{
		x2 = S*x2;
	}
	end = clock();
	cost = end - start;
	cout << cost << endl;

	Matrix<float, Dynamic, Dynamic, RowMajor> y;// y是n*3的矩阵,
	y = x2;

	if (y.cols() > 3)  y = y.leftCols(3);
	if (y.rows() == 0 || y.cols() != 3) return;

	M.upload(y.data(), y.rows(), nullptr, 0, nullptr); //显示变换后的y

	 // TODO 3: compute reconstruction error
	MatX3f meshErro;
	vector<float> distance;
	for (size_t i = 0; i < N; i++)
	{
		double tmp = 0;
		tmp += ((y(i, 0) - meshX(i, 0)) * (y(i, 0) - meshX(i, 0)));
		tmp += ((y(i, 1) - meshX(i, 1)) * (y(i, 1) - meshX(i, 1)));
		tmp += ((y(i, 2) - meshX(i, 2)) * (y(i, 2) - meshX(i, 2)));
		distance.push_back(sqrt(tmp));
	}

	std::vector<float> err(meshX.rows(), 0);
	M.setVertexDataViz(distance.data());

}

void Smooth(int n)
{
	if (n < 0) {
		M.upload(meshX.data(), meshX.rows(), nullptr, 0, nullptr);//upload 应该是上传图像以显示
		return;
	}
	
	using namespace Eigen;
	using namespace std;
	eigen2matlab("x", meshX);
	std::cout << meshX.rows() << endl;

	// TODO, compute laplacian, and sent to Matlab
	int N = meshX.size() / 3;
	scalar2matlab("N", N);
	auto L = Laplacian(meshX, meshF);
	eigen2matlab("L", L);
	scalar2matlab("m", m);

	auto &eng = getMatEngine();
	//eng.eval("[E,D]=eig(L);");
	//eng.eval("E2 = E';");
	//eng.eval("x2 = E2*x;");
	//eng.eval("c=zeros(N,N);");
	//eng.eval("for j=1:N;c(j,j) = (1-1/2*D(j,j))^m;end;");
	//eng.eval("x3 = (x2'*c)';");
	//eng.eval("y = E*x3");

	double start, end, cost;
	start = clock();
	eng.eval("S = eye(N) -(1.0/2.0)*L");
	eng.eval("y = x");
	eng.eval("for j=1:m;y=S*y;end;");
	end = clock();
	cost = end - start;
	cout << cost << endl;
	
	Matrix<float, Dynamic, Dynamic, RowMajor> y;// y是n*3的矩阵,
	matlab2eigen("single(y)", y, true); //single为了节约空间
	if (y.cols() > 3)  y = y.leftCols(3);
	if (y.rows() == 0 || y.cols() != 3) return;

	M.upload(y.data(),  y.rows(), nullptr, 0, nullptr); //显示变换后的y


	// TODO 3: compute reconstruction error
	MatX3f meshErro;
	vector<float> distance;
	for (size_t i = 0; i < N; i++)
	{
		double tmp = 0;
		tmp += ((y(i, 0) - meshX(i, 0)) * (y(i, 0) - meshX(i, 0)));
		tmp += ((y(i, 1) - meshX(i, 1)) * (y(i, 1) - meshX(i, 1)));
		tmp += ((y(i, 2) - meshX(i, 2)) * (y(i, 2) - meshX(i, 2)));
		distance.push_back(sqrt(tmp));
	}

	std::vector<float> err(meshX.rows(), 0);
	M.setVertexDataViz(distance.data());
	
}

int mousePressButton;
int mouseButtonDown;
int mousePos[2];

bool msaa = true;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (msaa) glEnable(GL_MULTISAMPLE);
	else glDisable(GL_MULTISAMPLE);

	glViewport(0, 0, viewport[2], viewport[3]);
	M.draw(viewport);

	if (showATB) TwDraw();
	glutSwapBuffers();

	//glFinish();
}

void onKeyboard(unsigned char code, int x, int y)
{
	if (!TwEventKeyboardGLUT(code, x, y)) {
		switch (code) {
		case 17:
			exit(0);
		case 'f':
			glutFullScreenToggle();
			break;
		case ' ':
			showATB = !showATB;
			break;
		}
	}

	glutPostRedisplay();
}

void onMouseButton(int button, int updown, int x, int y)
{
	if (!showATB || !TwEventMouseButtonGLUT(button, updown, x, y)) {
		mousePressButton = button;
		mouseButtonDown = updown;

		mousePos[0] = x;
		mousePos[1] = y;
	}

	glutPostRedisplay();
}


void onMouseMove(int x, int y)
{
	if (!showATB || !TwEventMouseMotionGLUT(x, y)) {
		if (mouseButtonDown == GLUT_DOWN) {
			if (mousePressButton == GLUT_MIDDLE_BUTTON) {
				M.moveInScreen(mousePos[0], mousePos[1], x, y, viewport);
			}
			else if (mousePressButton == GLUT_LEFT_BUTTON) {
				const float s[2] = { 2.f / viewport[2], 2.f / viewport[3] };
				auto r = Quat<float>(M.mQRotate)*Quat<float>::trackball(x*s[0] - 1, 1 - y*s[1], s[0] * mousePos[0] - 1, 1 - s[1] * mousePos[1]);
				std::copy_n(r.q, 4, M.mQRotate);
			}
		}
	}

	mousePos[0] = x; mousePos[1] = y;

	glutPostRedisplay();
}


void onMouseWheel(int wheel_number, int direction, int x, int y)
{
	M.mMeshScale *= direction > 0 ? 1.1f : 0.9f;
	glutPostRedisplay();
}

int initGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(960, 960);
	glutInitWindowPosition(200, 50);
	glutCreateWindow(argv[0]);

	// !Load the OpenGL functions. after the opengl context has been created
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
		return -1;

	glClearColor(1.f, 1.f, 1.f, 0.f);

	glutReshapeFunc([](int w, int h) { viewport[2] = w; viewport[3] = h; TwWindowSize(w, h); });
	glutDisplayFunc(display);
	glutKeyboardFunc(onKeyboard);
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutMouseWheelFunc(onMouseWheel);
	glutCloseFunc([]() {exit(0); });
	return 0;
}


void createTweakbar()
{
	TwBar *bar = TwGetBarByName("MeshViewer");
	if (bar)    TwDeleteBar(bar);

	//Create a tweak bar
	bar = TwNewBar("MeshViewer");
	TwDefine(" MeshViewer size='220 200' color='0 128 255' text=dark alpha=128 position='5 5'"); // change default tweak bar size and color

	TwAddVarRO(bar, "#Vertex", TW_TYPE_INT32, &M.nVertex, " group='Mesh View'");
	TwAddVarRO(bar, "#Face", TW_TYPE_INT32, &M.nFace, " group='Mesh View'");
	TwAddVarRW(bar, "Point Size", TW_TYPE_FLOAT, &M.pointSize, " group='Mesh View' ");
	TwAddVarRW(bar, "Vertex Color", TW_TYPE_COLOR4F, M.vertexColor.data(), " group='Mesh View' help='mesh vertex color' ");

	TwAddVarRW(bar, "Edge Width", TW_TYPE_FLOAT, &M.edgeWidth, " group='Mesh View' ");
	TwAddVarRW(bar, "Edge Color", TW_TYPE_COLOR4F, M.edgeColor.data(), " group='Mesh View' help='mesh edge color' ");

	TwAddVarRW(bar, "Face Color", TW_TYPE_COLOR4F, M.faceColor.data(), " group='Mesh View' help='mesh face color' ");

	TwDefine(" MeshViewer/'Mesh View' opened=false ");

	TwAddVarRW(bar, "m", TW_TYPE_INT32, &m, " group='Laplace Smooth' help='#basis for Laplace Smooth' min=1 ");
	TwAddButton(bar, "Smooth!", [](void *clientData) { Smooth(m); }, nullptr, "group='Laplace Smooth' ");
	TwAddButton(bar, "SmoothEigen!", [](void *clientData) { SmoothEigen(m); }, nullptr, "group='Laplace Smooth' ");
	TwAddButton(bar, "SmoothCUDA!", [](void *clientData) { SmoothCUDA(m); }, nullptr, "group='Laplace Smooth' ");
	TwAddButton(bar, "Original", [](void *clientData) { Smooth(-1); }, nullptr, "group='Laplace Smooth' ");
	TwAddVarRW(bar, "Plot Error", TW_TYPE_BOOLCPP, &M.vizVtxData, " group='Laplace Smooth' ");
}

int main(int argc, char *argv[])
{
	if (initGL(argc, argv)) {
		fprintf(stderr, "!Failed to initialize OpenGL!Exit...");
		exit(-1);
	}

	MyMesh::buildShaders();

	std::vector<float> x;
	std::vector<int> f;
	const char* meshpath = argc > 1 ? argv[1] : "horse.obj";
	readObj(meshpath, x, f);

	meshX = Eigen::Map<MatX3f>(x.data(), x.size() / 3, 3);
	meshF = Eigen::Map<MatX3i>(f.data(), f.size() / 3, 3);

	M.upload(x.data(), x.size() / 3, f.data(), f.size() / 3, nullptr);

	//////////////////////////////////////////////////////////////////////////
	TwInit(TW_OPENGL_CORE, NULL);
	//Send 'glutGetModifers' function pointer to AntTweakBar;
	//required because the GLUT key event functions do not report key modifiers states.
	TwGLUTModifiersFunc(glutGetModifiers);
	glutSpecialFunc([](int key, int x, int y) { TwEventSpecialGLUT(key, x, y); glutPostRedisplay(); }); // important for special keys like UP/DOWN/LEFT/RIGHT ...
	TwCopyStdStringToClientFunc([](std::string& dst, const std::string& src) {dst = src; });

	createTweakbar();

	//////////////////////////////////////////////////////////////////////////
	atexit([] { TwDeleteAllBars();  TwTerminate(); });

	glutTimerFunc(1, [](int) { getMatEngine().connect(""); }, 0);

	//////////////////////////////////////////////////////////////////////////
	glutMainLoop();

	return 0;
}
