#include "renderingwidget.h"
#include <QKeyEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <iostream>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QTextCodec>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <algorithm>
#include "mainwindow.h"
#include "ArcBall.h"
#include "globalFunctions.h"
//#include "HE_mesh/Mesh3D.h"


RenderingWidget::RenderingWidget(QWidget *parent, MainWindow* mainwindow)
: QGLWidget(parent), ptr_mainwindow_(mainwindow), eye_distance_(5.0),
has_lighting_(false), is_draw_point_(true), is_draw_edge_(false), is_draw_face_(false), is_draw_texture_(false)
{
	ptr_arcball_ = new CArcBall(width(), height());
	//ptr_mesh_ = new Mesh3D();
	mesh = *(new MyMesh());
	is_load_texture_ = false;
	is_draw_axes_ = false;
	is_read_ = false;

	eye_goal_[0] = eye_goal_[1] = eye_goal_[2] = 0.0;
	eye_direction_[0] = eye_direction_[1] = 0.0;
	eye_direction_[2] = 1.0;
}

RenderingWidget::~RenderingWidget()
{
	SafeDelete(ptr_arcball_);
	//SafeDelete(&mesh);
}

void RenderingWidget::initializeGL()
{
	glClearColor(0.3, 0.3, 0.3, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);

	SetLight();

}

void RenderingWidget::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.001, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderingWidget::paintGL()
{
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (has_lighting_)
	{
		SetLight();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	register vec eyepos = eye_distance_*eye_direction_;
	gluLookAt(eyepos[0], eyepos[1], eyepos[2],
		eye_goal_[0], eye_goal_[1], eye_goal_[2],
		0.0, 1.0, 0.0);
	glPushMatrix();

	glMultMatrixf(ptr_arcball_->GetBallMatrix());

	Render();
	glPopMatrix();
}

void RenderingWidget::timerEvent(QTimerEvent * e)
{
	updateGL();
}

void RenderingWidget::mousePressEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		ptr_arcball_->MouseDown(e->pos());
		break;
	case Qt::MidButton:
		current_position_ = e->pos();
		break;
	default:
		break;
	}

	updateGL();
}
void RenderingWidget::mouseMoveEvent(QMouseEvent *e)
{
	switch (e->buttons())
	{
		setCursor(Qt::ClosedHandCursor);
	case Qt::LeftButton:
		ptr_arcball_->MouseMove(e->pos());
		break;
	case Qt::MidButton:
		eye_goal_[0] -= 4.0*GLfloat(e->x() - current_position_.x()) / GLfloat(width());
		eye_goal_[1] += 4.0*GLfloat(e->y() - current_position_.y()) / GLfloat(height());
		current_position_ = e->pos();
		break;
	default:
		break;
	}

	updateGL();
}
void RenderingWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		break;
	default:
		break;
	}
	updateGL();
}
void RenderingWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		ptr_arcball_->MouseUp(e->pos());
		setCursor(Qt::ArrowCursor);
		break;

	case Qt::RightButton:
		break;
	default:
		break;
	}
}

void RenderingWidget::wheelEvent(QWheelEvent *e)
{
	eye_distance_ += e->delta()*0.001;
	eye_distance_ = eye_distance_ < 0 ? 0 : eye_distance_;

	updateGL();
}

void RenderingWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

void RenderingWidget::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

void RenderingWidget::Render()
{
	DrawAxes(is_draw_axes_);

	DrawPoints(is_draw_point_);
	DrawEdge(is_draw_edge_);
	DrawFace(is_draw_face_);
	DrawTexture(is_draw_texture_);
}

void RenderingWidget::SetLight()
{
	static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat mat_shininess[] = { 50.0 };
	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	static GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
	static GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void RenderingWidget::SetBackground()
{
	QColor color = QColorDialog::getColor(Qt::white, this, tr("background color"));
	GLfloat r = (color.red()) / 255.0f;
	GLfloat g = (color.green()) / 255.0f;
	GLfloat b = (color.blue()) / 255.0f;
	GLfloat alpha = color.alpha() / 255.0f;
	glClearColor(r, g, b, alpha);
	updateGL();
}

void RenderingWidget::ReadMesh()
{
	is_read_ = true;
	QString filename = QFileDialog::
		getOpenFileName(this, tr("Read Mesh"),
		"..", tr("Meshes (*.obj)"));

	if (filename.isEmpty())
	{
		emit(operatorInfo(QString("Read Mesh Failed!")));
		return;
	}
	//中文路径支持
	QTextCodec *code = QTextCodec::codecForName("gd18030");
	QTextCodec::setCodecForLocale(code);

	/*QByteArray byfilename = filename.toLocal8Bit();
	ptr_mesh_->LoadFromOBJFile(byfilename.data());*/
	OpenMesh::IO::Options opt_read = 0x0040;
	mesh.request_vertex_texcoords2D();
	if (!OpenMesh::IO::read_mesh(mesh, filename.toStdString(),opt_read))
	{
		std::cerr << "Cannot Open mesh to file '1.obj'" << std::endl;
		//return 1;
	}
	mesh.request_vertex_normals();
	OpenMesh::IO::Options opt;
	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
	{
		// we need face normals to update the vertex normals  
		mesh.request_face_normals();

		// let the mesh update the normals  
		mesh.update_normals();

		// dispose the face normals, as we don't need them anymore  
		mesh.release_face_normals();
	}
	//	m_pMesh->LoadFromOBJFile(filename.toLatin1().data());

	emit(operatorInfo(QString("Read Mesh from") + filename + QString(" Done")));
	//emit(meshInfo(ptr_mesh_->num_of_vertex_list(), ptr_mesh_->num_of_edge_list(), ptr_mesh_->num_of_face_list()));

	updateGL();
}

void RenderingWidget::WriteMesh()
{
	if (mesh.n_vertices() == 0)
	{
		emit(QString("The Mesh is Empty !"));
		return;
	}
	QString filename = QFileDialog::
		getSaveFileName(this, tr("Write Mesh"),
		"..", tr("Meshes (*.obj)"));

	if (filename.isEmpty())
		return;

	//ptr_mesh_->WriteToOBJFile(filename.toLatin1().data());

	if (!OpenMesh::IO::write_mesh(mesh, filename.toStdString()))
	{
		std::cerr << "Cannot Write mesh to file 'output.obj'" << std::endl;
		//return 1;
	}

	emit(operatorInfo(QString("Write Mesh to ") + filename + QString(" Done")));
}

void RenderingWidget::LoadTexture()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Texture"),
		"..", tr("Images(*.bmp *.jpg *.png *.jpeg)"));
	if (filename.isEmpty())
	{
		emit(operatorInfo(QString("Load Texture Failed!")));
		return;
	}


	glGenTextures(1, &texture_[0]);
	QImage tex1, buf;
	if (!buf.load(filename))
	{
		//        QMessageBox::warning(this, tr("Load Fialed!"), tr("Cannot Load Image %1").arg(filenames.at(0)));
		emit(operatorInfo(QString("Load Texture Failed!")));
		return;
		/*
		QImage dummy(128, 128, QImage::Format_ARGB32);
		dummy.fill(Qt::green);
		buf = dummy;
		*/
	}

	tex1 = QGLWidget::convertToGLFormat(buf);
	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex1.width(), tex1.height(),
		GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits());

	is_load_texture_ = true;
	emit(operatorInfo(QString("Load Texture from ") + filename + QString(" Done")));
}

void RenderingWidget::CheckDrawPoint(bool bv)
{
	is_draw_point_ = bv;
	updateGL();
}
void RenderingWidget::CheckDrawEdge(bool bv)
{
	is_draw_edge_ = bv;
	updateGL();
}
void RenderingWidget::CheckDrawFace(bool bv)
{
	is_draw_face_ = bv;
	updateGL();
}
void RenderingWidget::CheckLight(bool bv)
{
	has_lighting_ = bv;
	updateGL();
}
void RenderingWidget::CheckDrawTexture(bool bv)
{
	is_draw_texture_ = bv;
	if (is_draw_texture_)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	updateGL();
}
void RenderingWidget::CheckDrawAxes(bool bV)
{
	is_draw_axes_ = bV;
	updateGL();
}

void RenderingWidget::DrawAxes(bool bV)
{
	if (!bV)
		return;
	//x axis
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.7, 0.0, 0.0);
	glEnd();
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//y axis
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.7, 0.0);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0, 0.7, 0);
	glRotatef(90, -1.0, 0.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//z axis
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.0, 0.7);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0, 0, 0.7);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
}

void RenderingWidget::DrawPoints(bool bv)
{
	if (!bv || &mesh == NULL)
		return;
	if (mesh.n_vertices() == 0)
	{
		return;
	}

	glBegin(GL_POINTS);

	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
	{
		//Get the vertices of the point
		auto point = mesh.point(it.handle());
		auto normal = mesh.normal(it.handle());
		glNormal3fv(normal.data());
		glVertex3fv(point.data());
	}

	glEnd();
}

void RenderingWidget::DrawEdge(bool bv)
{
	if (!bv || &mesh == NULL)
		return;

	if (mesh.n_vertices() == 0)
	{
		return;
	}

	glBegin(GL_LINES);

	for (auto it = mesh.halfedges_begin(); it != mesh.halfedges_end(); ++it)
	{
		auto point_from = mesh.point(mesh.from_vertex_handle(it.handle()));
		auto point_to = mesh.point(mesh.to_vertex_handle(it.handle()));
		auto normal1 = mesh.normal(mesh.from_vertex_handle(it.handle()));
		auto normal2 = mesh.normal(mesh.from_vertex_handle(it.handle()));
		glNormal3fv(normal1.data());
		glVertex3fv(point_from.data());
		glNormal3fv(normal2.data());
		glVertex3fv(point_to.data());
	}

	glEnd();

	//const std::vector<HE_face *>& faces = *(ptr_mesh_->get_faces_list());
	//for (size_t i = 0; i != faces.size(); ++i)
	//{
	//	glBegin(GL_LINE_LOOP);
	//	HE_edge *pedge(faces.at(i)->pedge_);
	//	do
	//	{
	//		glNormal3fv(pedge->pvert_->normal().data());
	//		glVertex3fv(pedge->pvert_->position().data());

	//		pedge = pedge->pnext_;

	//	} while (pedge != faces.at(i)->pedge_);
	//	glEnd();
	//}
}

void RenderingWidget::DrawFace(bool bv)
{
	if (!bv || &mesh == NULL)
		return;

	if (mesh.n_vertices() == 0)
	{
		return;
	}

	glBegin(GL_TRIANGLES);

	for (auto it1 = mesh.faces_begin(); it1 != mesh.faces_end(); ++it1)
	{
		//OpenMesh::Vec3f point_vec3f;

		
		for (auto it2 = mesh.fv_begin(it1); it2 != mesh.fv_end(it1); ++it2)
		{
			auto point = mesh.point(it2.handle());
			auto normal = mesh.normal(it2.handle());
			//point_vec3f = point;
			/*OpenMesh::FaceHandle face = it2.handle();
			for (auto it = mesh.fh_begin(face); it != mesh.fh_end(face);it++)*/

			glNormal3fv(normal.data());
			glVertex3fv(point.data());
		}
		std::cout << std::endl;
	}

	glEnd();

	//for (size_t i = 0; i != faces.size(); ++i)
	//{
	//	HE_edge *pedge(faces.at(i)->pedge_);
	//	do
	//	{
	//		glNormal3fv(pedge->pvert_->normal().data());
	//		glVertex3fv(pedge->pvert_->position().data());

	//		pedge = pedge->pnext_;

	//	} while (pedge != faces.at(i)->pedge_);
	//}

	
}

void RenderingWidget::DrawTexture(bool bv)
{
	if (!bv)
		return;
	if (mesh.n_faces() == 0 || !is_load_texture_)
		return;

	/*测试

	//默认使用球面纹理映射，效果不好
	ptr_mesh_->SphereTex();

	const std::vector<HE_face *>& faces = *(ptr_mesh_->get_faces_list());

	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i != faces.size(); ++i)
	{
		HE_edge *pedge(faces.at(i)->pedge_);
		do
		{
			//请在此处绘制纹理，添加纹理坐标即可
			glTexCoord2fv(pedge->pvert_->texCoord_.data());
			glNormal3fv(pedge->pvert_->normal().data());
			glVertex3fv(pedge->pvert_->position().data());

			pedge = pedge->pnext_;

		} while (pedge != faces.at(i)->pedge_);
	}

	glEnd();

	*/
	
	MyMesh::TexCoord2D;
	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glBegin(GL_TRIANGLES);

	for (auto it1 = mesh.faces_begin(); it1 != mesh.faces_end(); ++it1)
	{

		for (auto it2 = mesh.fv_begin(it1); it2 != mesh.fv_end(it1); ++it2)
		{
			auto point = mesh.point(it2.handle());
			auto normal = mesh.normal(it2.handle());
			auto texcoord = mesh.texcoord2D(it2.handle());

			glTexCoord2fv(texcoord.data());
			glNormal3fv(normal.data());
			glVertex3fv(point.data());
		}
		std::cout << std::endl;
	}

	glEnd();


	//glBegin(GL_TRIANGLES);

	//for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
	//{
	//	//Get the vertices of the point
	//	auto point = mesh.point(it.handle());
	//	auto normal = mesh.normal(it.handle());
	//	auto texcoord = mesh.texcoord2D(it.handle());
	//	glTexCoord2fv(texcoord.data());
	//	glNormal3fv(normal.data());
	//	glVertex3fv(point.data());
	//}

	//glEnd();

}

void RenderingWidget::Uniform()
{
	MeshParameterization meshParameterization(mesh);
	meshParameterization.uniform();
	mesh = meshParameterization.mesh;
	updateGL();
}

void RenderingWidget::WeightedLeastSquares()
{
	MeshParameterization meshParameterization(mesh);
	meshParameterization.weightedLeastSquares();
	mesh = meshParameterization.mesh;
	updateGL();
}

void RenderingWidget::ShapePreserving()
{
	MeshParameterization meshParameterization(mesh);
	meshParameterization.shapePreserving();
	mesh = meshParameterization.mesh;
	updateGL();
}


