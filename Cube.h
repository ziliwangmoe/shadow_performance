//
//  Cube class
//  The constructor sets the position, size and rotation
//  All parameters are assigned default values
//
#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

class Cube: public Object
{
private:
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
public:
	Cube();
	void display(QOpenGLShaderProgram *program);                          //  Render the cube
};

#endif
