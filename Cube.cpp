//
//  Cube class
//
#include "Cube.h"
#include <QVector2D>
#include <QVector3D>

struct VertexData_Tangent
{
	QVector3D position;
	QVector2D texCoord;
	QVector3D normal;
	QVector3D tangent;
};


Cube::Cube() : indexBuf(QOpenGLBuffer::IndexBuffer)
{
	arrayBuf.create();
	indexBuf.create();
	VertexData_Tangent vertices[] = {
	   // Vertex data for face 0
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f) },  // v0
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f) }, // v1
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(0.0f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f) },  // v2
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f) }, // v3

	   // Vertex data for face 1
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.0f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) }, // v4
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) }, // v5
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) },  // v6
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.33f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) }, // v7

	   // Vertex data for face 2
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f) }, // v8
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f) },  // v9
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f) }, // v10
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f) },  // v11

	   // Vertex data for face 3
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) }, // v12
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) },  // v13
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.66f, 0.5f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) }, // v14
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(1.0f, 0.5f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f) },  // v15

	   // Vertex data for face 4
		{ QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v16
		{ QVector3D(1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v17
		{ QVector3D(-1.0f, -1.0f, 1.0f), QVector2D(0.33f, 0.5f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v18
		{ QVector3D(1.0f, -1.0f, 1.0f), QVector2D(0.66f, 0.5f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v19

	   // Vertex data for face 5
		{ QVector3D(-1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v20
		{ QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.66f, 0.5f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v21
		{ QVector3D(-1.0f, 1.0f, -1.0f), QVector2D(0.33f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }, // v22
		{ QVector3D(1.0f, 1.0f, -1.0f), QVector2D(0.66f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f) }  // v23
   };

   GLushort indices[] = {
	   0, 1, 2, 3, 3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
	   4, 4, 5, 6, 7, 7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
	   8, 8, 9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
	   12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
	   16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
	   20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
   };
   //GLushort indices[] = {0,1,2,1,2,3};
   //GLushort indices[] = { 2, 1, 0, 3, 2, 1 };
   // Transfer vertex data to VBO 0
   arrayBuf.bind();
   arrayBuf.allocate(vertices, 24 * sizeof(VertexData_Tangent));

   // Transfer index data to VBO 1
   indexBuf.bind();
   indexBuf.allocate(indices, 34 * sizeof(GLushort));
	
}

void Cube::display(QOpenGLShaderProgram *program)
{
	// Tell OpenGL which VBOs to use
	arrayBuf.bind();
	indexBuf.bind();

	// Offset for position
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

	// Draw cube geometry using indices from VBO 1
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
   
}
