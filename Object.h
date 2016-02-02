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
public:
	QVector3D trans = QVector3D(0.0, 0.0,0.0);
	QVector3D scale = QVector3D(1.0, 1.0, 1.0);
public:
	QMatrix4x4 modelMat; 
	virtual void display(QOpenGLShaderProgram *program) = 0; 
	virtual ~Object() {}; 
	void applyToMat();
protected:
	
};

#endif
