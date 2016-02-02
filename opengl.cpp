//
//  OpenGL opengl widget
//
#include "opengl.h"
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
#define DEPTH_TEXTURE_SIZE 1024

opengl::opengl(QWidget* parent)
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
	renderOnce = false;
	m_fps = 0;
	fps_counter = 0;
	x_thres = 0.5;
	y_thres = 0.5;
	isInline = 0;
	isInt = 0;
	iterCount = 1000;
}
void opengl::reset()
{
	th = ph = 0;
	dim = 3;
	Projection();
	//  Request redisplay
	updateGL();
}
void opengl::setLightMove(bool on)
{
	move = on;
	updateGL();
}
void opengl::setShader(int sel)
{
	mode = sel;
	//  Request redisplay
	updateGL();
}
void opengl::setX(int X)
{
	x_thres = X / 100.0f;
	updateGL();
}
void opengl::setY(int Y)
{
	y_thres = Y / 100.0f;
	updateGL();
}
void opengl::setZoom(int Z)
{
	iterCount = Z * 100;
	updateGL();
}
void opengl::setPerspective(int on)
{
	isInt = on;
	updateGL();
}
void opengl::setObject(int type)
{
	isInline = type;
	updateGL();
}
void opengl::setAntiAlias(int _isAnti)
{
	isAntiAlias = _isAnti;
}

void opengl::setupDepthMap()
{
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depth_texture, 0);
	//glDrawBuffer(GL_NONE);

}

void opengl::rendDepthMap(QVector3D light_position)
{
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);
	
	QMatrix4x4 scene_model_matrix;
	QMatrix4x4 light_view_matrix;
	QMatrix4x4 light_projection_matrix;
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1024, 1024);
	shader[0].bind();
	QVector3D Y = QVector3D(0.0, 1.0, 0.0);
	light_view_matrix.lookAt(light_position, QVector3D(0.0, 0.0, 0.0), Y);
	//light_projection_matrix.perspective(90, 1.0, 0.5, 10);
	light_projection_matrix.frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f);
	vpMatrix_light = light_projection_matrix*light_view_matrix;
	mvpMatrix_light = light_projection_matrix*light_view_matrix*cube->modelMat;
	shader[0].setUniformValue("MVPMatrix", mvpMatrix_light);
	cube->display(&shader[0]);
	mvpMatrix_light = light_projection_matrix*light_view_matrix*obj->modelMat;
	shader[0].setUniformValue("MVPMatrix", mvpMatrix_light);
	obj->display(&shader[0]);

	glDisable(GL_POLYGON_OFFSET_FILL);

}

void opengl::showDepthMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenW, screenH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	shader[1].bind();
	shader[1].setUniformValue("depthTex", 0);
	depthScreen->display(&shader[1]);
}

void opengl::renderShadow(QVector3D light_position){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenW, screenH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	QMatrix4x4 scale_bias_matrix = QMatrix4x4(0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f);
	QMatrix4x4 shadow_matrix = scale_bias_matrix * vpMatrix_light;
	shader[2].bind();
	QVector3D ambient(0.3, 0.3, 0.3);
	QVector3D diffuse(0.3, 0.3, 0.3);
	QVector3D specular(0.3, 0.3, 0.3);

	shader[2].setUniformValue("material_ambient", ambient);
	shader[2].setUniformValue("material_diffuse", diffuse);
	shader[2].setUniformValue("material_specular", specular);
	shader[2].setUniformValue("material_specular_power", 2.0f);
	shader[2].setUniformValue("depth_texture", 0);

	shader[2].setUniformValue("shadow_matrix", shadow_matrix);
	shader[2].setUniformValue("view_matrix", viewMat);
	shader[2].setUniformValue("projection_matrix", projMat);
	QVector4D light_position_4d(light_position.x(), light_position.y(), light_position.z(), 1.0);
	shader[2].setUniformValue("light_position", viewMat*light_position_4d);

	shader[2].setUniformValue("model_matrix", cube->modelMat);
	cube->display(&shader[2]);
	shader[2].setUniformValue("model_matrix", obj->modelMat);
	obj->display(&shader[2]);
}

void opengl::renderPerformTest()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenW, screenH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader[3].bind();
	shader[3].setUniformValue("x_thres", (float)(x_thres * 2 - 1.0));
	shader[3].setUniformValue("y_thres", (float)(y_thres * 2 - 1.0));
	shader[3].setUniformValue("isInline", isInline);
	shader[3].setUniformValue("isInt", isInt);
	shader[3].setUniformValue("iterCount", iterCount);
	depthScreen->display(&shader[3]);
}

void opengl::initializeGL()
{

	if (init) return;
	init = true;
	initializeOpenGLFunctions();
	//  Load shaders
	Shader(0, ":/depth.vert", ":/depth.frag");
	Shader(1, ":/white.vert", ":/white.frag");
	Shader(2, ":/shadow.vert", ":/shadow.frag");
	Shader(3, ":/performance.vert", ":/performance.frag");


	ball = new Sphere();
	cube = new Cube();
	cube->scale = QVector3D(3.0, 0.5, 3.0);
	cube->trans = QVector3D(0.0, -2.0, 0.0);
	cube->applyToMat();
	objects.push_back(new WaveOBJ("suzanne.obj", ":/"));
	obj = objects[0];

	depthScreen = new Cube();

	move = true;
	timer.setInterval(1);
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer.start();
	time.start();
	setupDepthMap();
	SetViewMat();
	Projection();
	time_recorded = clock();
}
void opengl::resizeGL(int width, int height)
{
	screenW = width;
	screenH = height;
	glViewport(0, 0, (GLint)width, (GLint)height);
	asp = height ? width / (float)height : 1;
	Projection();
}
void opengl::paintGL()
{
	if (fps_counter >= 30){
		fps_counter = 0;
		clock_t t_cur = clock();
		clock_t t_delta = t_cur - time_recorded;
		time_recorded = t_cur;
		m_fps = floor(CLOCKS_PER_SEC / (float)t_delta*30);
		emit fps(QString::number(m_fps));
		float deltaTime = 1.0/m_fps*1000;
		emit info1(QString::number(deltaTime));
	}
	fps_counter++;
	
	//if (renderOnce){
	//	return;
	//}
	renderOnce = true;
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
	ball->trans = QVector3D(lightPosition.x(), lightPosition.y(), lightPosition.z());
	//shader[1].bind();
	//shader[1].setUniformValue("mvpMatrix", projMat * viewMat* ball->modelMat);
	//ball->display(&shader[1]);
	if (mode == 0 || mode == 1){
		rendDepthMap(ball->trans);
		if (mode == 0){
			renderShadow(ball->trans);
		}
		else{
			showDepthMap();
		}
		
	}
	else if (mode == 2){
		renderPerformTest();
	}
	glDisable(GL_DEPTH_TEST);

	emit angles(QString::number(th) + "," + QString::number(ph));
	
}
void opengl::Fatal(QString message)
{
	QMessageBox::critical(this, "opengl", message);
	QApplication::quit();
}
void opengl::Projection()
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
void opengl::SetViewMat()
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
void opengl::mousePressEvent(QMouseEvent* e)
{
	mouse = true;
	pos = e->pos();  //  Remember mouse location
}
void opengl::mouseReleaseEvent(QMouseEvent*)
{
	mouse = false;
}
void opengl::mouseMoveEvent(QMouseEvent* e)
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
void opengl::wheelEvent(QWheelEvent* e)
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
void opengl::Shader(int k, QString vert, QString frag)
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
