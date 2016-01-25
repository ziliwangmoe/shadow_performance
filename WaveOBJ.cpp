//
//  Wave OBJ class
//
//  The STL vector template class is used to manage arrays
//
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QPixmap>
#include <QGLWidget>
#include <vector>
#include "WaveOBJ.h"

using namespace std;

//  Read float
static float Read1(QStringList s)
{
	float v;
	bool OK;
	v = s[1].toFloat(&OK);
	if (!OK) throw "Error reading float from " + s.join(" ");
	return v;
}

//  Read Vec2
static Vec2 Read2(QStringList s)
{
	Vec2 v;
	bool OKx, OKy;
	v.x = s[1].toFloat(&OKx);
	v.y = s[2].toFloat(&OKy);
	if (!OKx || !OKy) throw "Error reading Vec2 from " + s.join(" ");
	return v;
}

//  Read Vec3
static Vec3 Read3(QStringList s)
{
	Vec3 v;
	bool OKx, OKy, OKz;
	v.x = s[1].toFloat(&OKx);
	v.y = s[2].toFloat(&OKy);
	v.z = s[3].toFloat(&OKz);
	if (!OKx || !OKy || !OKz) throw "Error reading Vec3 from " + s.join(" ");
	return v;
}

//
//  Load materials from file
//
void WaveOBJ::LoadMaterial(const QString& file, const QString& path)
{
	////  Open file or return with warning on error
	//QFile f(path + file);
	//if (!f.open(QFile::ReadOnly | QFile::Text)) throw "Cannot open material file " + path + file;
	//QTextStream in(&f);

	////  Process materials file
	//QString name;
	//QString line;
	//while (line = in.readLine(), !line.isNull())
	//{
	//	//  Split line into words
	//	QStringList words = line.simplified().split(" ");
	//	//  New material
	//	if (words.size() == 2 && words[0] == "newmtl")
	//	{
	//		name = words[1];
	//		Material m;
	//		m.Ns = 0;
	//		m.d = 0;
	//		m.map = 0;
	//		mat[name] = m;
	//	}
	//	//  If no material short circuit here
	//	else if (name.isNull())
	//	{
	//	}
	//	//  Emission color
	//	else if (words.size() == 4 && words[0] == "Ke")
	//		mat[name].Ke = Read3(words);
	//	//  Ambient color
	//	else if (words.size() == 4 && words[0] == "Ka")
	//		mat[name].Ka = Read3(words);
	//	//  Diffuse color
	//	else if (words.size() == 4 && words[0] == "Kd")
	//		mat[name].Kd = Read3(words);
	//	//  Specular color
	//	else if (words.size() == 4 && words[0] == "Ks")
	//		mat[name].Ks = Read3(words);
	//	//  Material Shininess
	//	else if (words.size() == 2 && words[0] == "Ns")
	//		mat[name].Ns = Read1(words);
	//	//  Textures (must be BMP - will fail if not)
	//	else if (words.size() == 2 && words[0] == "map_Kd")
	//	{
	//		unsigned int tex;
	//		glGenTextures(1, &tex);
	//		glBindTexture(GL_TEXTURE_2D, tex);
	//		QImage img(path + words[1]);
	//		QImage rgba = QGLWidget::convertToGLFormat(img);
	//		glTexImage2D(GL_TEXTURE_2D, 0, 4, rgba.width(), rgba.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.bits());
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		mat[name].map = tex;
	//	}
	//	//  Ignore line if we get here
	//}
	//f.close();
}

//
//  Set material
//
void WaveOBJ::SetMaterial(const QString& name)
{
	////  Search materials for a matching name
	//if (!mat.contains(name)) throw "Unknown material " + name;

	////  Set material colors
	//setColor(mat[name].Ka, mat[name].Kd, mat[name].Ks, mat[name].Ke, mat[name].Ns);

	////  Bind texture if specified
	//if (mat[name].map)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, mat[name].map);
	//}
	//else
	//	glDisable(GL_TEXTURE_2D);
}



WaveOBJ::WaveOBJ(const char* file, const QString& path)
{
	//  Initialize location
	translate(0, 0, 0);
	rotate(0, 1, 0, 0);
	scale(1);
	//  Initialize color and scale
	R = G = B = s = 1;

	//  Open file
	QFile f(path + file);
	if (!f.open(QFile::ReadOnly | QFile::Text)) throw QString("Cannot open file ") + file;
	QTextStream in(&f);
	arrayBuf.create();

	mat.clear();
	QVector<Vec3> V, N;
	QVector<Vec2> T;
	vector<VertexData> dataAll;
	QString line;
	while (line = in.readLine(), !line.isNull())
	{
		QStringList words = line.simplified().split(" ");
		if (words.size() == 4 && words[0] == "v")
			V.push_back(Read3(words));
		else if (words.size() == 4 && words[0] == "vn")
			N.push_back(Read3(words));
		else if (words.size() == 3 && words[0] == "vt")
			T.push_back(Read2(words));
		else if (words.size() > 3 && words[0] == "f")
		{
			vertexCount += words.size()-1;
			for (int k = 1; k < words.size(); k++)
			{
				int Kv = 0, Kt = 0, Kn = 0;
				QStringList parts = words[k].split('/');
				Kv = parts[0].toInt();
				Kt = parts[1].toInt();
				Kn = parts[2].toInt();
				// throw QString("Invalid facet ")+words[k].c_str();
				//  Check index
				if (Kv<-(int)V.count() || Kv>(int)V.count()) throw "Vertex " + QString::number(Kv) + " out of range 1-" + QString::number(V.count());
				if (Kn<-(int)N.count() || Kn>(int)N.count()) throw "Normal " + QString::number(Kn) + " out of range 1-" + QString::number(N.count());
				if (Kt<-(int)T.count() || Kt>(int)T.count()) throw "Texture " + QString::number(Kt) + " out of range 1-" + QString::number(T.count());
				//  Adjust "from end" references
				if (Kv < 0) Kv = V.count() + Kv + 1;
				if (Kn < 0) Kn = N.count() + Kn + 1;
				if (Kt < 0) Kt = T.count() + Kt + 1;

				VertexData tempVert = { QVector3D(V[Kv - 1].x, V[Kv - 1].y, V[Kv - 1].z),
					QVector2D(Kt ? T[Kt - 1].x : 0, Kt ? T[Kt - 1].y:0),
					QVector3D(Kn ? N[Kn - 1].x : 0, Kn ? N[Kn - 1].y : 0, Kn ? N[Kn - 1].z : 0) };
				dataAll.push_back(tempVert);
			}
		}
		else if (words.size() == 2 && words[0] == "usemtl")
			SetMaterial(words[1]);
		else if (words.size() == 2 && words[0] == "mtllib")
			LoadMaterial(words[1], path);
	}
	f.close();

	arrayBuf.bind();
	arrayBuf.allocate(&dataAll[0], dataAll.size() * sizeof(VertexData));

}
void WaveOBJ::display(QOpenGLShaderProgram *program)
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

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

//
//  Set scaling
//
void WaveOBJ::scale(float S)
{
	s = S;
}

//
//  Set color
//
void WaveOBJ::color(float r, float g, float b)
{
	R = r;
	G = g;
	B = b;
}

//
//  Set rotation
//
void WaveOBJ::rotate(float ang, float dX, float dY, float dZ)
{
	th = ang;
	dx = dX;
	dy = dY;
	dz = dZ;
}
