//
//  Sphere class
//  The constructor sets the position, radius and color
//  All parameters are assigned default values
//
#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

class Sphere : public Object
{
private:
	QOpenGLBuffer arrayBuf;
	float R, G, B;     //  Color
	int vertCount = 0;
public:
	Sphere(float x = 0, float y = 0, float z = 0, float R = 1, float G = 1, float B = 1);
	void display(QOpenGLShaderProgram *program);                          //  Render the sphere
};

#endif
