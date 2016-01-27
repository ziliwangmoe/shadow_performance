//
//  Sphere class
//
#include "Sphere.h"
#include <math.h>
#include <vector>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
using namespace std;
struct VertexData_Tangent
{
	QVector3D position;
	QVector2D texCoord;
	QVector3D normal;
	QVector3D tangent;
};

static QVector3D Polar2Car(double th, double ph){
	return QVector3D(Cos(th)*Cos(ph), Sin(th)*Cos(ph), Sin(ph));
}

static VertexData_Tangent Vertex(double th, double ph, QVector3D _tan)
{
	double s = th / 360;
	double t = ph / 180 + 0.5;
	QVector3D pos = Polar2Car(th, ph);
	QVector3D tang = Polar2Car(th, -ph);
	VertexData_Tangent vert_t = { pos, QVector2D(s, t), pos, tang };
	return vert_t;
}

static QVector3D getTangent(double th_ori, double ph_ori, double th_end, double ph_end){
	QVector3D pos_ori = Polar2Car(th_ori, ph_ori);
	QVector3D pos_end = Polar2Car(th_end, ph_end);
	QVector3D tangent = pos_end - pos_ori;
	tangent.normalize();
	return tangent;
}

Sphere::Sphere(float x, float y, float z,float Rc, float Gc, float Bc) :
Object(x, y, z)
{
	R = Rc;
	G = Gc;
	B = Bc;
	scale = 0.3;
	arrayBuf.create();
	vector<VertexData_Tangent> vList;
	const int inc = 10;
	for (int ph = -90; ph < 90; ph += inc)
	{
		for (int th = 0; th <= 360; th += 2 * inc)
		{
			vertCount+=6;
			QVector3D  tangent = getTangent(th, ph, th + 2 * inc, ph);
			vList.push_back(Vertex(th, ph, tangent));
			vList.push_back(Vertex(th + 2 * inc, ph, tangent));
			vList.push_back(Vertex(th, ph + inc, tangent));
			tangent = getTangent(th, ph + inc, th + 2 * inc, ph);
			vList.push_back(Vertex(th, ph + inc, tangent));
			vList.push_back(Vertex(th + 2 * inc, ph, tangent));
			vList.push_back(Vertex(th + 2 * inc, ph + inc, tangent));
		}
	}
	arrayBuf.bind();
	arrayBuf.allocate(&vList[0], vList.size() * sizeof(VertexData_Tangent));
	

}

void Sphere::display(QOpenGLShaderProgram *program)
{
	arrayBuf.bind();
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData_Tangent));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData_Tangent));

	offset += sizeof(QVector2D);

	int noramlLocation = program->attributeLocation("a_normal");
	program->enableAttributeArray(noramlLocation);
	program->setAttributeBuffer(noramlLocation, GL_FLOAT, offset, 3, sizeof(VertexData_Tangent));

	offset += sizeof(QVector3D);

	int tangentLocation = program->attributeLocation("a_tangent");
	program->enableAttributeArray(tangentLocation);
	program->setAttributeBuffer(tangentLocation, GL_FLOAT, offset, 3, sizeof(VertexData_Tangent));

	glDrawArrays(GL_TRIANGLES, 0, vertCount);

}
