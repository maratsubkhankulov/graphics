#ifndef _rt_H
#define _rt_H

using namespace std;
#include <cmath>
#include <vector>
#include <GL/glut.h>

//
// Sample code for physics simulation
//


// Implements cloth simulation


class Vector3f;
class Triangle;
class TriangleMesh;


class Vector3f {

	float _item[3];

	public:

	float & operator [] (int i) {
		return _item[i];
    	}

	Vector3f(float x, float y, float z) 
	{  _item[0] = x ; _item[1] = y ; _item[2] = z; };

	Vector3f() {};


	Vector3f & operator = (Vector3f & obj) 
	{
		_item[0] = obj[0];
		_item[1] = obj[1];
		_item[2] = obj[2];

		return *this;
	};

	Vector3f & operator += (Vector3f & obj) 
	{
		_item[0] += obj[0];
		_item[1] += obj[1];
		_item[2] += obj[2];

		return *this;
	};

	float normalize() {
		float ret = sqrt(_item[0]*_item[0] + _item[1]*_item[1] + _item[2]*_item[2]); 

		for (int i = 0; i < 3; i++) _item[i]/=ret;

		return ret;
	}
};

ostream & operator << (ostream & stream, Vector3f & obj) 
{
	stream << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ';
	return stream;
};

float operator * (Vector3f & v1, Vector3f & v2)  
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]; 
};


/*
float fmax(float f1,float f2, float f3) {
	float f = f1;

	if (f < f2) f = f2;
	if (f < f3) f = f3;

	return f;
};

float fmin(float f1,float f2, float f3) {
	float f = f1;

	if (f > f2) f = f2;
	if (f > f3) f = f3;

	return f;
};

*/



class Volume {

private:
	int _size[3];
	unsigned short ** _volume ; 

public:
	void rasterize(int x, int y, int z);
	void load(char * filename);
	unsigned short volume(int x, int y, int z) {
		if (x > (_size[0] - 2) || y > (_size[1] - 2) || z > (_size[2] - 2) || x < 0 || y < 0 || z < 0)
		{
			return 0;
		} else 
		{
			return _volume[y][z * _size[2] + x];
		}
	}; 

	int sizex() {return _size[0]; };
	int sizey() {return _size[1]; };
	int sizez() {return _size[2]; };
};

class TransferControlPoint {

public:
	float colour[3];
	float alpha;
	int scalar;

	TransferControlPoint(float r, float g, float b, int scalar)
	{
		colour[0] = r; colour[1] = g; colour[2] = b;
		alpha = 1.f;
		this->scalar = scalar;
	}

	TransferControlPoint(float a, int scalar)
	{
		colour[0] = 0; colour[1] = 0; colour[2] = 0;
		alpha = a;
		this->scalar = scalar;
	}
};


#endif //demo3
