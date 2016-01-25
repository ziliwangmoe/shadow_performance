//
//  Generic Object class
//
#include "Object.h"

Object::Object(float x,float y,float z)
{

}
void Object::translate(float x,float y,float z)
{
	modelMat.setToIdentity();
	modelMat.translate(x, y, z);
	modelMat.scale(scale);
	
}
void Object::setColor(Color c)
{
}

void Object::setColor(Color a,Color d,Color s,Color e,float Ns)
{
}

void Object::texture(QString file)
{

}

//
//  Enable texture
//
void Object::EnableTex()
{

}

//
//  Disable texture
//
void Object::DisableTex()
{

}
