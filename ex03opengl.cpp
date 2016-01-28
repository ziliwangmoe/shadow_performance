//
//  OpenGL Ex03opengl widget
//
#include "ex03opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "Cube.h"
#include "WaveOBJ.h"
//#include "Teapot.h"
#include <math.h>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define Tan(x) tan(3.1415926/180*(x))
#include <QVector3D>

Ex03opengl::Ex03opengl(QWidget* parent)
	: QGLWidget(parent)
{
	mode = 0;
	init = false;
	mouse = false;
	asp = 1;
	dim = 3;
	fov = 0;
	th = ph = 0;
	x0 = y0 = 0;
	z0 = 1;
	zh = 0;
	fixZ = 6;
	isAntiAlias = 1;
}
void Ex03opengl::reset()
{
	th = ph = 0;
	dim = 3;
	Projection();
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setLightMove(bool on)
{
	move = on;
	updateGL();
}
void Ex03opengl::setShader(int sel)
{
	mode = sel;
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setX(double X)
{
	x0 = X;
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setY(double Y)
{
	y0 = Y;
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setZoom(double Z)
{
	z0 = Z;
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setPerspective(int on)
{
	fov = on ? 55 : 0;
	Projection();
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setObject(int type)
{
	obj = objects[type];
	//  Request redisplay
	updateGL();
}
void Ex03opengl::setAntiAlias(int _isAnti)
{
	isAntiAlias = _isAnti;
}

void Ex03opengl::initializeGL()
{
	if (init) return;
	init = true;

	// Load cube.png image
	texture = new QOpenGLTexture(QImage(":/pi.png").mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);


	//  Load shaders
	Shader(0, ":/noise.vert", ":/noise.frag");
	Shader(1, ":/ex03a.vert", ":/ex03a.frag");
	Shader(2, ":/ex03b.vert", ":/ex03b.frag");
	Shader(3, ":/bump.vert", ":/bump.frag");


	ball = new Sphere();

	objects.push_back(new Cube());

	Sphere* ball_obj = new Sphere();
	objects.push_back(ball_obj);

	WaveOBJ* suza = 0;
	try
	{
		suza = new WaveOBJ("suzanne.obj", ":/");
	}
	catch (QString err)
	{
		Fatal("Error loading object\n" + err);
	}
	if (suza)
	{
		suza->color(1, 1, 0);
		objects.push_back(suza);
	}

	//  Set initial object
	obj = objects[0];

	//  Start 100 fps timer connected to updateGL
	move = true;
	timer.setInterval(10);
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer.start();
	time.start();

	SetViewMat();
	Projection();
	//glEnable(GL_CULL_FACE);
}
void Ex03opengl::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
	asp = height ? width / (float)height : 1;
	Projection();
}
void Ex03opengl::paintGL()
{
	//  Wall time (seconds)
	float t = 0.001*time.elapsed();
	if (move) zh = fmod(90 * t, 360);

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//  Translate intensity to color vectors
	QVector3D ambient(0.3, 0.3, 0.3);
	QVector3D diffuse(0.3, 0.3, 0.3);
	QVector3D specular(0.3, 0.3, 0.3);
	QVector4D lightPosition((float)(3 * Cos(zh)), 2.0, (float)(3 * Sin(zh)), 1.0);
	ball->translate(lightPosition.x(), lightPosition.y(), lightPosition.z());
	shader[1].bind();
	shader[1].setUniformValue("mvpMatrix", projMat * viewMat* ball->modelMat);
	ball->display(&shader[1]);

	float dotR = 0.16;
	//  Apply shader
	texture->bind();
	shader[mode].bind();
	//  Dimensions
	QVector3D loc(x0, y0, 1 / z0);
	shader[mode].setUniformValue("loc", loc);
	shader[mode].setUniformValue("time", t);
	QMatrix4x4 tmep = projMat * viewMat;
	shader[mode].setUniformValue("mvpMatrix", projMat * viewMat);
	shader[mode].setUniformValue("mvMatrix", viewMat);
	shader[mode].setUniformValue("normalMatrix", viewMat);
	shader[mode].setUniformValue("lightPos", viewMat*lightPosition);
	shader[mode].setUniformValue("ambient", ambient);
	shader[mode].setUniformValue("diffuse", diffuse);
	shader[mode].setUniformValue("specular", specular);
	shader[mode].setUniformValue("shininess", 2.0f);
	shader[mode].setUniformValue("texture", 0);
	shader[mode].setUniformValue("radius", dotR);
	shader[mode].setUniformValue("isAntiAlias", isAntiAlias);

	if (obj) obj->display(&shader[mode]);

	if (mode) shader[mode].release();
	glDisable(GL_DEPTH_TEST);

	//  Emit angles to display
	emit angles(QString::number(th) + "," + QString::number(ph));
	//  Emit centers to display
	emit zoom(QString::number((int)z0));
}
void Ex03opengl::Fatal(QString message)
{
	QMessageBox::critical(this, "Ex03opengl", message);
	QApplication::quit();
}
void Ex03opengl::Projection()
{
	QMatrix4x4 projMat_t;
	float zn = dim / 4;
	float zf = dim * 8;

	if (fov == 0){
		projMat_t.ortho(-dim*asp, dim*asp, -dim, dim, zn, zf);
		//projMat.setRow(0, QVector4D(dim, 0.0, 0.0, 0.0));
		//projMat.setRow(1, QVector4D(0.0, asp*dim, 0.0, 0.0));
		//projMat.setRow(2, QVector4D(0.0, 0.0, -1 / (zf - zn), zn / (zf - zn)));
		//projMat.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
	}
	else{
		float fov_t = 1 / tan((float)fov / 2 / 180.0*3.1415);
		float nearPlanH = zn*Tan(fov / 2);
		projMat_t.frustum(-nearPlanH*asp*dim / 3, nearPlanH*asp*dim / 3, -nearPlanH*dim / 3, nearPlanH*dim / 3, zn, zf);
		//projMat_t.perspective(fov, asp, zn, zf);

		//projMat.setRow(0, QVector4D(fov_t*dim/3, 0.0, 0.0, 0.0));
		//projMat.setRow(1, QVector4D(0.0, fov_t*asp*dim/3, 0.0, 0.0));
		//projMat.setRow(2, QVector4D(0.0, 0.0, zn / (zf - zn), -zf*zn / (zf - zn)));
		//projMat.setRow(3, QVector4D(0.0, 0.0, 1.0, 0.0));
	}
	projMat = projMat_t;

}
void Ex03opengl::SetViewMat()
{
	QMatrix4x4 temp;
	temp.lookAt(QVector3D(Cos(ph)*Sin(th)*fixZ, Sin(ph)*fixZ, Cos(ph)*Cos(th)*fixZ), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	//temp.rotate(th, 0, 1, 0);
	//temp.rotate(ph, 1, 0, 0);
	//temp.translate(0, 0, fixZ);
	viewMat = temp;
}
/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
void Ex03opengl::mousePressEvent(QMouseEvent* e)
{
	mouse = true;
	pos = e->pos();  //  Remember mouse location
}
void Ex03opengl::mouseReleaseEvent(QMouseEvent*)
{
	mouse = false;
}
void Ex03opengl::mouseMoveEvent(QMouseEvent* e)
{
	if (mouse)
	{
		QPoint d = e->pos() - pos;  //  Change in mouse location
		pos = e->pos();           //  Remember new location
		th = (th - d.x()) % 360;      //  Translate x movement to azimuth
		ph = (ph + d.y()) % 360;      //  Translate y movement to elevation
		SetViewMat();
		updateGL();               //  Request redisplay
	}
}
void Ex03opengl::wheelEvent(QWheelEvent* e)
{
	//  Zoom out
	if (e->delta() < 0)
		dim -= 0.1;
	//  Zoom in
	else if (dim > 1)
		dim += 0.1;
	//  Request redisplay
	Projection();
	updateGL();
}
void Ex03opengl::Shader(int k, QString vert, QString frag)
{
	//  Vertex shader
	if (vert.length() && !shader[k].addShaderFromSourceFile(QOpenGLShader::Vertex, vert))
		Fatal("Error compiling " + vert + "\n" + shader[k].log());
	//  Fragment shader
	if (frag.length() && !shader[k].addShaderFromSourceFile(QOpenGLShader::Fragment, frag))
		Fatal("Error compiling " + frag + "\n" + shader[k].log());
	//  Link
	if (!shader[k].link())
		Fatal("Error linking shader\n" + shader[k].log());
}
