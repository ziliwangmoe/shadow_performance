//
//  OpenGL opengl Widget
//

#ifndef opengl_H
#define opengl_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include "Object.h"
#include "Sphere.h"
#include "Cube.h"
#include "QOpenGLFunctions_3_3_Core"
#include <time.h> 

class opengl : public QGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
private:
	GLuint depth_texture;
	GLuint depth_fbo;
	QOpenGLTexture *depthTex;
	QOpenGLFramebufferObject *dephtBuf;
	QOpenGLShaderProgram *depthShader;
	Cube* cube;
	float screenW, screenH;
	bool renderOnce;
	bool    init;      //  Initialized
	bool    move;      //  Light animation
	float   zh;        //  Light position
	float   x0, y0;     //  Object center
	float   z0;        //  Zoom
	int     mode;      //  Display mode
	int     th, ph;     //  Display angles
	float   fixZ;
	bool    mouse;     //  Mouse pressed
	int    isAntiAlias;
	QPoint  pos;       //  Mouse position
	int     fov;       //  Field of view
	double  dim;       //  Display size
	double  asp;       //  Sceen aspect ratio
	Object* obj;       //  Object
	Sphere* ball;
	Cube *depthScreen;
	QOpenGLShaderProgram shader[4]; //  Shaders
	QVector<Object*> objects;   //  Objects
	QTimer           timer;     //  Timer for animations
	QElapsedTimer    time;      //  Track elapsed time
	QMatrix4x4 projMat;
	QMatrix4x4 viewMat;
	QOpenGLTexture *texture;
	QMatrix4x4 vpMatrix_light;
	QMatrix4x4 mvpMatrix_light;
	int m_fps;
	clock_t time_recorded;
	int fps_counter; //used in only cal. fps every 10 frame
	float x_thres;
	float y_thres;
	int isInline;
	int isInt;
	int iterCount;
public:
	opengl(QWidget* parent = 0);                  //  Constructor
	QSize sizeHint() const { return QSize(400, 400); } //  Default size of widget
	public slots:
	void setShader(int sel);               //  Slot to set shader
	void setPerspective(int on);           //  Slot to set projection type
	void setObject(int type);              //  Slot to set displayed object
	void setAntiAlias(int isAnti);
	void setX(int X);                   //  Slot to set X position
	void setY(int Y);                   //  Slot to set Y position
	void setZoom(int Z);                //  Slot to set Zoom level
	void setLightMove(bool on);            //  Slot to set light movement
	void reset();                          //  Reset view
signals:
	void angles(QString text);             //  Signal for view angles
	void fps(QString text);               //  Signal for zoom
	void info1(QString text);
	void info2(QString text);
protected:
	void initializeGL();                   //  Initialize widget
	void resizeGL(int width, int height);  //  Resize widget
	void paintGL();                        //  Draw widget
	void mousePressEvent(QMouseEvent*);    //  Mouse pressed
	void mouseReleaseEvent(QMouseEvent*);  //  Mouse released
	void mouseMoveEvent(QMouseEvent*);     //  Mouse moved
	void wheelEvent(QWheelEvent*);         //  Mouse wheel
private:
	void Fatal(QString message);            //  Error handler
	void Projection();                      //  Update projection
	void Shader(int k, QString vert, QString frag);  //  Create shader
	void SetViewMat();
	void setupDepthMap();
	void rendDepthMap(QVector3D light_position);
	void showDepthMap();
	void renderShadow(QVector3D light_position);
	void renderPerformTest();
};

#endif
