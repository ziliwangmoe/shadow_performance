//
//  Sphere class
//
#include "Sphere.h"
#include <math.h>
#include <vector>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
using namespace std;
static VertexData Vertex(double th, double ph)
{
	double s = th / 360;
	double t = ph / 180 + 0.5;
	double x = Cos(th)*Cos(ph);
	double y = Sin(th)*Cos(ph);
	double z = Sin(ph);

	VertexData vert_t = { QVector3D(x, y, z), QVector2D(s, t), QVector3D(x, y, z) };
	return vert_t;
}
Sphere::Sphere(float x, float y, float z,float Rc, float Gc, float Bc) :
Object(x, y, z)
{
	R = Rc;
	G = Gc;
	B = Bc;
	scale = 0.3;
	arrayBuf.create();
	vector<VertexData> vList;
	const int inc = 10;
	for (int ph = -90; ph < 90; ph += inc)
	{
		for (int th = 0; th <= 360; th += 2 * inc)
		{
			vertCount+=6;
			vList.push_back(Vertex(th, ph));
			vList.push_back(Vertex(th + 2 * inc, ph));
			vList.push_back(Vertex(th, ph + inc));
			vList.push_back(Vertex(th, ph + inc));
			vList.push_back(Vertex(th + 2 * inc, ph));
			vList.push_back(Vertex(th + 2 * inc, ph + inc));
		}
	}
	arrayBuf.bind();
	arrayBuf.allocate(&vList[0], vList.size() * sizeof(VertexData));
	

}

void Sphere::display(QOpenGLShaderProgram *program)
{
	arrayBuf.bind();
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	offset += sizeof(QVector2D);

	int noramlLocation = program->attributeLocation("a_normal");
	program->enableAttributeArray(noramlLocation);
	program->setAttributeBuffer(noramlLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	glDrawArrays(GL_TRIANGLES, 0, vertCount);

}
