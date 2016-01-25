//
//  Object class
//  Dummy generic object class
//  Location of object is common to all objects
//  Translation is defined for all objects, but must
//  be implemented in the display function of each object
//  The display function is virtual and must be reimplemented
//
#ifndef OBJECT_H
#define OBJECT_H

#include "Type.h"
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct VertexData
{
	QVector3D position;
	QVector2D texCoord;
	QVector3D normal;
};

class Object
{
protected:
	
	unsigned int tex;                        //  Default texture
	Object(float x = 0, float y = 0, float z = 0);   //  Constructor
	float x0, y0, z0;
	float scale=1.0;
public:
	QMatrix4x4 modelMat;
	void translate(float x, float y, float z); //  Set translation
	void texture(QString file);              //  Set default texture
	virtual void display(QOpenGLShaderProgram *program) = 0;                //  Render the object
	virtual ~Object() {};                    //  Do nothing destructor
protected:
	void setColor(Color c);
	void setColor(Color a, Color d, Color s, Color e, float Ns);
	void EnableTex();
	void DisableTex();
};

#endif
