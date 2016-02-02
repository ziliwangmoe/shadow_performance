//
//  Generic Object class
//
#include "Object.h"
void Object::applyToMat()
{
	modelMat.setToIdentity();
	modelMat.translate(trans);
	modelMat.scale(scale);
}

