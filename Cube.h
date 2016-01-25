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
	float Sx,Sy,Sz;  //  Dimensions in X/Y/Z directions
	float Th;        //  Rotation around Y axis
   
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
public:
	Cube();
	void rotate(float th);                   //  Set rotation
	void scale(float dx,float dy,float dz);  //  Set scale
	void display(QOpenGLShaderProgram *program);                          //  Render the cube
};

#endif
