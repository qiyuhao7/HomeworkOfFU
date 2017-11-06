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
#include "objloader.h"
#include "glprogram.h"
#include "MyImage.h"
#include "VAOImage.h"
#include "VAOMesh.h"
#include "trackball.h"

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

struct node
{
	double value;
	int index;
};

bool cmp(struct node a, struct node b)
{
	if (a.value > b.value)
	{
		return true;
	}
	return false;
}

void compressMesh(int n)
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
	//MatrixXd L(n, n);
	int N = meshX.size()/3;
	MatrixXd L = MatrixXd::Zero(N, N);
	//处理第一行
	L(0, 0) = 1.0;
	L(0, 1) = -0.5;
	L(0, N - 1) = -0.5;
	//中间行

	for (size_t i = 1; i < N - 1; i++)
	{
		L(i, i - 1) = -0.5;
		L(i, i) = 1.0;
		L(i, i + 1) = -0.5;

	}
	//最后一行
	L(N - 1, 0) = -0.5;
	L(N - 1, N - 2) = -0.5;
	L(N - 1, N - 1) = 1.0;


	//int count[2000];
	//for (size_t i = 0; i < N; i++)
	//{
	//	count[i] = 0;
	//}
	//for (size_t i = 0; i < N; i++)
	//{
	//	count[meshF(i, 0)]++;
	//	count[meshF(i, 1)]++;
	//	count[meshF(i, 2)]++;
	//}

	//node* a = new node[N];
	//for (size_t i = 0; i < N; i++)
	//{
	//	a[i].value = 0;
	//	a[i].index = i+1;
	//}
	//for (size_t i = 0; i < N; i++)
	//{
	//	a[meshF(i, 0)].value++;
	//	a[meshF(i, 1)].value++;
	//	a[meshF(i, 2)].value++;
	//}
	//std::sort(a, a + N,cmp);

	//std::vector<int> key;

	//for (size_t i = 0; i < N; i++)
	//{
	//	key.push_back(a[i].index);
	//}

	//vector2matlab("key", key);

	// L = Laplacian(meshX, meshF);
	eigen2matlab("L", L);
	scalar2matlab("n", n);
	// TODO 2: compress mesh in Matlab

	auto &eng = getMatEngine();
	//eng.eval("if ~issorted(diag(D));[V, D] = eig(L);[D, I] = sort(diag(D));V = V(:, I);end");
	//eng.eval("L(1:15,1:15)");
	eng.eval("[V,D]=eig(L);");
	//eng.eval("[D_sort, index] = sort(diag(D), 'decend');");
	//eng.eval("D_sort = D_sort(index);");
	//eng.eval("sortrows(V,1)");
	//eng.eval("D(1:10,1:10)");
	eng.eval("E=V;");
	//eng.eval("key");
	//eng.eval("E(1:10,1:10)");
	eng.eval("E2 = E';");
	eng.eval("N = length(x)");
	eng.eval("x2 = E2*x;");
	//eng.eval("[x2,index] = sortrows(x2,1)");
	//eng.eval("E3(:,index) = E;");
	MatX3f x2;
	matlab2eigen("x2", x2);

	node* a = new node[N];
	for (size_t i = 0; i < N; i++)
	{
		a[i].index = i+1;
	}
	for (size_t i = 0; i < N; i++)
	{
		double t = x2(i, 0)*x2(i, 0) + x2(i, 1)*x2(i, 1) + x2(i, 1)*x2(i, 1);
			a[i].value = sqrt(t);
	}

	std::sort(a, a + N,cmp);

	std::vector<int> key;

	for (size_t i = 0; i < N; i++)
	{
		key.push_back(a[i].index);
	}

	vector2matlab("key", key);
	eng.eval("key");
	eng.eval("sum=zeros(N,3);");
	eng.eval("for j = 1:n;c = E(:,key(j))*x2(key(j),:); sum = sum +c;end;");
	//eng.eval("for j = 1:n; t=1+(j-1)*N/n;t=uint32(t); c = E(:,t)*x2(t,:); sum = sum +c;end;");
	//eng.eval("for k = 1:n; t=1+(k-1)*n/N; t=uint32(t); x2(k,:) = x(t,:);end;");
	//eng.eval(" sort(a,1)");
	//eng.eval("if ~issorted(diag(D));[V, D] = eig(A);[D, I] = sort(diag(D));V = V(:, I);end");
	//eng.eval("sortrows(a,3)");
	//eng.eval("");
	eng.eval("y = sum");
	
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

	TwAddVarRW(bar, "n Basis", TW_TYPE_INT32, &nEigs, " group='Spectral Compression' help='#basis for spectral compression' min=1 ");
	TwAddButton(bar, "Compress!", [](void *clientData) { compressMesh(nEigs); }, nullptr, "group='Spectral Compression' ");
	TwAddButton(bar, "Original", [](void *clientData) { compressMesh(-1); }, nullptr, "group='Spectral Compression' ");
	TwAddVarRW(bar, "Plot Error", TW_TYPE_BOOLCPP, &M.vizVtxData, " group='Spectral Compression' ");
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
