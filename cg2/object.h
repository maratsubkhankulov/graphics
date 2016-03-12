#include "teapot.h"
#include <math.h>

#ifndef OBJECT_H
#define OBJECT_H


/*Set up object variables*/
float objectAngle = M_PI;
float objectScale = 1;
float objectShear = 0;
float objectR = 0.7; float objectG = 0.7; float objectB = 1.0;
Vector4f objectPosition = Vector4f(0,0,0,1);

void rotateObject(float n)
{
	objectAngle += n;
}

void scaleObject(float s)
{
	objectScale += s;
}

#endif
