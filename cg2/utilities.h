#include "teapot.h"
#include "camera.h"

#ifndef UTILITIES_H
#define UTILITIES_H

float min(float a, float b, float c)
{
	float min = 0;
	min = a < b ? a : b;
	min = min < c ? min : c;
	return min;
};

float max(float a, float b, float c)
{
	float max = 0;
	max = a > b ? a : b;
	max = max > c ? max : c;
	return max;
};

float getBarycentricComponent(double x, double y, double xa, double ya, double xb, double yb)
{
	return (ya - yb)*x + (xb - xa)*y + xa*yb - xb*ya;
};

bool inRange(double from, double to, double n)
{
	return from <= n && n <= to;
};

bool isBarycentric(double x, double y, double x0, double y0, double x1, double y1, double x2, double y2)
{
	double alpha, beta, gamma = 0;
	alpha = getBarycentricComponent(x,y,x1,y1,x2,y2)/getBarycentricComponent(x0,y0,x1,y1,x2,y2);
	beta = getBarycentricComponent(x,y,x2,y2,x0,y0)/getBarycentricComponent(x1,y1,x2,y2,x0,y0);
	gamma = getBarycentricComponent(x,y,x0,y0,x1,y1)/getBarycentricComponent(x2,y2,x0,y0,x1,y1);
	return 0.0 < alpha && alpha < 1.0 && 0.0 < beta && beta < 1.0 && 0.0 < gamma && gamma < 1.0;
};

Vector3f getPlaneNormal(Vector3f a, Vector3f b, Vector3f c)
{
	return ((b-a).cross(c-a)).normalize();
}

float getZDepth(float z, float near, float far)
{
	return ((far+near)/(far-near) + ((-2)*far*near)/(far - near))/z;
}

float getDistanceBetweenPoints(float x0, float y0, float x1, float y1)
{
	return sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1));
}

double getZOnPlane(float x, float y, Vector4f a, Vector4f b, Vector4f c)
{
/*	alpha = getBarycentricComponent(x,y,b[0],b[1],c[0],c[1])/getBarycentricComponent(a[0],a[1],b[0],b[1],c[0],c[1]);
	beta = getBarycentricComponent(x,y,c[0],c[1],a[0],a[1])/getBarycentricComponent(b[0],b[1],c[0],c[1],a[0],a[1]);
	gamma = getBarycentricComponent(x,y,a[0],a[1],b[0],b[1])/getBarycentricComponent(c[0],c[1],a[0],a[1],b[0],b[1]);
	Vector3f n(alpha, beta, gamma);
	n.normalize();
	return (a[2]*n[0] + b[2]*n[1] + c[2]*n[2]);	*/
	double dA = getDistanceBetweenPoints(x,y,a[0],a[1]);
	double dB = getDistanceBetweenPoints(x,y,b[0],b[1]); 
	double dC = getDistanceBetweenPoints(x,y,c[0],c[1]);
	double sumD = dA + dB + dC;
	double cA = 1 - dA/sumD; double cB = 1 - dB/sumD; double cC = 1 - dC/sumD;
	return a[2]*cA + b[2]*cB + c[2]*cC;
}

void reflectVectorGivenNormal(Vector3f dir, Vector3f nor, Vector3f* ref)
{
	*ref = ((Vector3f)(nor*=2*(dir*nor))) -= dir;
	(*ref).normalize();
}

void interpolateNormals(int x, int y, float x0, float y0, float x1, float y1, float x2, float y2, 
			Vector3f nor1, Vector3f nor2, Vector3f nor3, Vector3f* normal)
{
	float alpha, beta, gamma = 0;	
	alpha = getBarycentricComponent(x,y,x1,y1,x2,y2)/getBarycentricComponent(x0,y0,x1,y1,x2,y2);
	beta = getBarycentricComponent(x,y,x2,y2,x0,y0)/getBarycentricComponent(x1,y1,x2,y2,x0,y0);
	gamma = getBarycentricComponent(x,y,x0,y0,x1,y1)/getBarycentricComponent(x2,y2,x0,y0,x1,y1);
	nor1*=alpha; nor2*=beta; nor3*=gamma;
	nor1+=nor2; nor1+=nor3;
	*normal = nor1;
	normal->normalize();
}

void findPointInTriangle(int x, int y, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, Vector3f *point)
{
	float alpha, beta, gamma = 0;	
	alpha = getBarycentricComponent(x,y,x1,y1,x2,y2)/getBarycentricComponent(x0,y0,x1,y1,x2,y2);
	beta = getBarycentricComponent(x,y,x2,y2,x0,y0)/getBarycentricComponent(x1,y1,x2,y2,x0,y0);
	gamma = getBarycentricComponent(x,y,x0,y0,x1,y1)/getBarycentricComponent(x2,y2,x0,y0,x1,y1);
	(*point)[0] = x; (*point)[1] = y; (*point)[2] = alpha*z0 + beta*z1 + gamma*z2;
}

bool areSimilarVector4f(Vector4f a, Vector4f b)
{
	return inRange(-1, 1, a[0] - b[0]) && inRange(-1, 1, a[1] - b[1]) && inRange(-1, 1, a[2] - b[2]);
}

#endif
