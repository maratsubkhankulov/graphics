#ifndef _rt_H
#define _rt_H

#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <cmath>
#include <limits>

using namespace std;

//
// Sample code for physics simulation
//


// Implements cloth simulation


class Vector3f;
class Vector4f;
class Matrix4by4f;
class Triangle;
class TriangleMesh;

class Matrix4by4f {
	
	float _item[16];

	public:

	Matrix4by4f(float ax, float ay, float az, float aw, 
		 float bx, float by, float bz, float bw,
		 float cx, float cy, float cz, float cw,
		 float dx, float dy, float dz, float dw)
	{ _item[0] = ax, _item[1] = ay, _item[2] = az, _item[3] = aw,
	  _item[4] = bx, _item[5] = by, _item[6] = bz, _item[7] = bw,
	  _item[8] = cx, _item[9] = cy, _item[10] = cz, _item[11] = cw,
	  _item[12] = dx, _item[13] = dy, _item[14] = dz, _item[15] = dw;};

	Matrix4by4f(){};

	inline
	float& operator() (unsigned row, unsigned col)
	{
		return _item[4*row + col];
	}

	inline
	float operator() (unsigned row, unsigned col) const
	{
		return _item[4*row + col];
	}	

};

class Vector3f {

	float _item[3];

	public:

	inline
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

	float operator * (Vector3f & obj) 
	{
		return _item[0]*obj[0] + _item[1]*obj[1] +_item[2]*obj[2];
	};


	bool areSame(float a, float b) {
		return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
	}


	bool operator == (Vector3f & obj)
	{
		return areSame(_item[0], obj[0]) && areSame(_item[1], obj[1]) && areSame(_item[2], obj[2]);
	}	
	
	Vector3f & operator -= (Vector3f & obj)
	{
		_item[0] -= obj[0];
		_item[1] -= obj[1];
		_item[2] -= obj[2];

		return *this;
	}

	Vector3f & operator *= (float n) 
	{
		_item[0] = _item[0]*n;
		_item[1] = _item[1]*n;
		_item[2] = _item[2]*n;
		return *this;
	};
	
	Vector3f & operator /= (float n) 
	{
		_item[0] = _item[0]/n;
		_item[1] = _item[1]/n;
		_item[2] = _item[2]/n;
		return *this;
	};
	
	Vector3f & cross(Vector3f & obj)
	{
		float a = _item[1]*obj[2] - _item[2]*obj[1];
		float b = _item[2]*obj[0] - _item[0]*obj[2];
		float c = _item[0]*obj[1] - _item[1]*obj[0];
		_item[0] = a; _item[1] = b; _item[2] = c;

		return *this;
	}

	Vector3f & operator - (Vector3f & obj)
	{
		_item[0] = _item[0] - obj[0];
		_item[1] = _item[1] - obj[1];
		_item[2] = _item[2] - obj[2];

		return *this;
	}

	Vector3f & normalize()
	{
		float norm = sqrt(_item[0]*_item[0] + _item[1]*_item[1] + _item[2]*_item[2]);
		_item[0] = _item[0]/norm;
		_item[1] = _item[1]/norm;
		_item[2] = _item[2]/norm;
		return *this;
	}
};

class Vector4f: public Vector3f
{
	float _item[4];
	
	public:

	inline
	float & operator [] (int i) {
		return _item[i];
    	}

	Vector4f(float x, float y, float z, float w) 
	{  _item[0] = x ; _item[1] = y ; _item[2] = z; _item[3] = w;};

	Vector4f() {};

	Vector4f & operator = (Vector4f & obj) 
	{
		_item[0] = obj[0];
		_item[1] = obj[1];
		_item[2] = obj[2];
		_item[3] = obj[3];

		return *this;
	};

	Vector4f & operator = (Vector3f & obj) 
	{
		_item[0] = obj[0];
		_item[1] = obj[1];
		_item[2] = obj[2];
		_item[3] = 1;

		return *this;
	};

	Vector4f & operator += (Vector4f & obj) 
	{
		_item[0] += obj[0];
		_item[1] += obj[1];
		_item[2] += obj[2];
		_item[3] += obj[3];

		return *this;
	};
};

void matDotVec(Matrix4by4f m, Vector4f * v)
{
	int i, j;
	float out[] = {0,0,0,0};
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			out[i] += m(i,j)*(*v)[j];
		}
	}

	(*v)[0] = out[0];
	(*v)[1] = out[1];
	(*v)[2] = out[2];
	(*v)[3] = out[3];
}

ostream & operator << (ostream & stream, Vector3f & obj) 
{
	stream << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ';
};

ostream & operator << (ostream & stream, Vector4f & obj) 
{
	stream << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ' << obj[3] << ' ';
};


class Triangle {
friend class TriangleMesh;

	int _vertex[3];
public:

	int getVertexIndex(int i) {return _vertex[i];}
	Triangle(int v1, int v2, int v3) 
	{
		_vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3;  
		
	};
};

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


class TriangleMesh 
{
	vector <Vector4f> _v;
	vector <Triangle> _trig;
	float _xmax, _xmin, _ymax, _ymin, _zmin, _zmax;

public: 
	TriangleMesh(char * filename) { loadFile(filename) ;};
	TriangleMesh() {};
	void loadFile(char * filename);

	int trigNum() { return _trig.size() ;};
	int getVertexNum() { return _v.size();};
	Triangle getTriangle(int i) { return _trig[i];}
	Vector4f v(int i) { return _v[i];};
	Vector4f* getVertex(int i) { return &_v[i];};		

	void getTriangleVertices(int i, Vector4f & v1, Vector4f & v2, Vector4f & v3)
	{
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]]; 
	}

	TriangleMesh & operator = (TriangleMesh & obj) 
	{
		Vector4f v1, v2, v3;
		for (int i = 0; i < obj.trigNum(); i++){
			obj.getTriangleVertices(i, v1, v2, v3);
			_v.push_back(Vector4f(v1[0],v1[1],v1[2],1));
			_v.push_back(Vector4f(v2[0],v2[1],v2[2],1));
			_v.push_back(Vector4f(v3[0],v3[1],v3[2],1));
			Triangle trig(i*3, i*3+1, i*3+2);
			_trig.push_back(trig);
		}

		return *this;
	};
			
};

#endif //_rt_H
