#ifndef _rt_H
#define _rt_H

#include <cmath>
#include <map>
#include <vector>
#include <cstring>
#include <GL/glut.h>

using namespace std;

const int KEY_W = 119;
const int KEY_S = 115;
const int KEY_A = 97;
const int KEY_D = 100;
const int KEY_O = 111;
const int KEY_P = 112;
const int KEY_Q = 113;
const int KEY_E = 101;
//
// Sample code for physics simulation
//


// Implements cloth simulation


class Vector3f;
class Triangle;
class TriangleMesh;
class Edge;



class Vector3f {

	float _item[3];

	public:

	float & operator [] (int i) {
		return _item[i];
    	}

	float operator [] (int i) const {
		return _item[i];
    	}


	Vector3f(float x, float y, float z) 
	{  _item[0] = x ; _item[1] = y ; _item[2] = z; };

	Vector3f() {};


	Vector3f & operator = (const Vector3f & obj) 
	{
		_item[0] = obj[0];
		_item[1] = obj[1];
		_item[2] = obj[2];

		return *this;
	};

	Vector3f & operator += (const Vector3f & obj) 
	{
		_item[0] += obj[0];
		_item[1] += obj[1];
		_item[2] += obj[2];

		return *this;
	};


	Vector3f & operator -= (const Vector3f & obj) 
	{
		_item[0] -= obj[0];
		_item[1] -= obj[1];
		_item[2] -= obj[2];

		return *this;
	};




	Vector3f & operator /= (const float & f) 
	{
		_item[0] /= f;
		_item[1] /= f;
		_item[2] /= f;

		return *this;
	};

	Vector3f & operator * (const float & f) 
	{
		_item[0] *= f;
		_item[1] *= f;
		_item[2] *= f;

		return *this;
	};

	float distance(Vector3f & v) 
	{
		float ret=0.f;

		ret = (v[0]-_item[0])*(v[0]-_item[0]) 
		    + (v[1]-_item[1])*(v[1]-_item[1])
		    + (v[2]-_item[2])*(v[2]-_item[2]);

		ret = sqrt(ret);

		return ret;
	}


	float dot(const Vector3f & input) const
	{
		return input._item[0]*_item[0] + input._item[1]*_item[1] + input._item[2]*_item[2] ; 
	}

	float normalize()
	{
		float ret = sqrt(_item[0]*_item[0] + _item[1]*_item[1] + _item[2]*_item[2]); 
		for (int i = 0; i < 3; i++) _item[i] /= ret;

		return ret;
	}




	friend Vector3f operator % ( const Vector3f & ob1, const Vector3f & ob2);
	friend Vector3f operator - ( const Vector3f & ob1, const Vector3f & ob2);
	friend Vector3f operator + ( const Vector3f & ob1, const Vector3f & ob2);

};

Vector3f operator % ( const Vector3f & ob1, const Vector3f & ob2)
{
    int i;              // loop counter
    Vector3f ret;            // return value
    int N_DV3 = 3;	
		
    for (i = 0; i < N_DV3; i++)
	ret._item[ i ] =
	    ob1[ ( i + 1 ) % N_DV3 ] * ob2[ ( i + 2 ) % N_DV3 ]
	    - ob2[ ( i + 1 ) % N_DV3 ] * ob1[ ( i + 2 ) % N_DV3 ];

    return ret;
}


Vector3f operator + (const Vector3f& v1, const Vector3f& v2)
{
    Vector3f v(v1);
    for (int i = 0; i < 3; i++)
	v[i] += v2[i];

    return v;
}

Vector3f operator - (const Vector3f& v1, const Vector3f& v2)
{

    Vector3f v(v1);
    for (int i = 0; i < 3 ; i++)
	v[i] -= v2[i];

    return v;
}




float distance(Vector3f & v1, Vector3f & v2)
{

	float ret;

	ret = sqrt((v2[0]-v1[0])*(v2[0]-v1[0]) + (v2[1]-v1[1])*(v2[1]-v1[1]) + (v2[2]-v1[2])*(v2[2]-v1[2]));

	return ret;
}


ostream & operator << (ostream & stream, Vector3f & obj) 
{
	stream << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ';
};




class Triangle {
friend class TriangleMesh;

	int _id;
	int _vertex[3];
	int _normal[3];
	int _edge[3];
	float _min, _max;
	float _color; 
	float _area;
	float _ratio;

public:

	Triangle(int v1, int v2, int v3, int n1, int n2, int n3) 
	{
		_vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3;  
		_normal[0] = n1;  _normal[1] = n2;  _normal[2] = n3;  
		
	};

	void setMorseMinMax(float min, float max)
	{
		_min = min; _max = max;
	}

	void getMorseMinMax(float & min, float & max)
	{
		min = _min; max = _max;
	}

	void setEdge(int e1, int e2, int e3)
	{
		_edge[0] = e1;
		_edge[1] = e2;
		_edge[2] = e3;
	};

	int edge(int i) { return _edge[i];};
	int id() { return _id;};
	void setColor(float f) { _color = f ;};
	float color() { return _color;};
};

class Edge {

	friend bool contain(Edge & e, map < pair <int, int> , Edge > & list) ;
	friend int edgeID(Edge & e, map < pair <int, int> , Edge > & list) ;
	friend bool contain(Edge & e, vector < Edge > & list) ;
	friend int edgeID(Edge & e, vector < Edge > & list) ;

	int _v1,_v2;
	vector <int> _trig_list;
	float length;
	int _id;

public: 

	Edge () {};

	Edge (int i, int j)
	{
		_v1 = i; _v2 = j;
	};

	bool operator == (Edge & e)
	{
		if (this->_v1 == e._v1 && this->_v2 == e._v2) return true;
		if (this->_v1 == e._v2 && this->_v2 == e._v1) return true;

		return false;
	}

	void add_triangle(int trig)
	{
		for (int i = 0; i < _trig_list.size(); i++) 
			if (trig == _trig_list[i]) return;

		_trig_list.push_back(trig);
	}

	void other_trig(const int trig, vector <int> & others) 
	{
		for (int i = 0; i < _trig_list.size(); i++) {
			if (_trig_list[i] == trig) continue; 
			else others.push_back(_trig_list[i]);
		}
	}


	void setId(int id) { _id = id;};
	int id() { return _id;};
	int v1() { return _v1;};
	int v2() { return _v2;};
	vector <int> getTrigList() { return _trig_list ;};
};




struct Node {
	int _id;

	vector<int> edges_to;     
	vector<float> edges_cost;   

	bool done;   
	float cost;    
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
	vector <Vector3f> _v;
	vector <Vector3f> _vn;
	vector <Triangle> _trig;
	vector <Node> _node;
	vector <Edge> _edge;

//	map <pair < int, int > , Edge > _edge;

	float _xmax, _xmin, _ymax, _ymin, _zmin, _zmax;



public: 
	TriangleMesh(char * filename) { loadFile(filename) ;};
	TriangleMesh() {};
	void loadFile(char * filename);

	int trigNum() { return _trig.size() ;};

	void getTriangleVertices(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3)
	{
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]]; 
	}

	void getTriangleVertices(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3, Vector3f & vIndices)
	{
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]]; 
		vIndices[0] = _trig[i]._vertex[0];
		vIndices[1] = _trig[i]._vertex[1];
		vIndices[2] = _trig[i]._vertex[2];
	}
	
	void getTriangleNormals(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3)
	{
		v1 = _vn[_trig[i]._normal[0]]; 
		v2 = _vn[_trig[i]._normal[1]]; 
		v3 = _vn[_trig[i]._normal[2]]; 
	}
		

	void getMorseValue(int i, float & v1, float & v2, float & v3)
	{
		v1 = _node[_trig[i]._vertex[0]].cost; 
		v2 = _node[_trig[i]._vertex[1]].cost; 
		v3 = _node[_trig[i]._vertex[2]].cost; 
	}

	float color(int i) { return _trig[i].color();};


	void setMorseMinMax(int i, float min, float max)
	{
		_trig[i].setMorseMinMax(min,max);
	}

	void getMorseMinMax(int i, float & min, float & max)
	{
		_trig[i].getMorseMinMax(min,max);
	}


	void calcTriangleArea() 
	{
		Vector3f v1,v2,v3;

		for (int i = 0 ;i < _trig.size(); i++) 
		{
			getTriangleVertices(i, v1,v2,v3);
			v3 -= v1;
			v2 -= v1;

			_trig[i]._area = 0.5f*sqrt(v3.dot(v3)*v2.dot(v2) - (v3.dot(v2)*(v3.dot(v2))));  
//			cout << "trig " << i << " v2 " << v2 << " v3 " << v3 << " area = " << _trig[i]._area << endl;
		}
	}
};


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
	Matrix4by4f operator* (Matrix4by4f const& mat) const
	{
		Matrix4by4f m = Matrix4by4f(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		for(int row=0;row<4;row++){
			for(int column=0;column<4;column++){
			    for(int n=0;n<4;n++){
				m(row,column)+= _item[4*row+n]*mat(n,column);
			    }
			}
		    }
		return m;
	}

	Vector3f operator* (Vector3f const& v) const
	{
		int i, j;
		Vector3f out = Vector3f(0,0,0);
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				out[i] += _item[4*i +j]*(v)[j];
			}
		}
		return out;		
	}
	
	Matrix4by4f & operator= (Matrix4by4f const& mat)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				_item[4*i + j] = mat(i,j);
		return *this;
	}

	Matrix4by4f & operator += (Matrix4by4f const & mat)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				_item[4*i + j] = mat(i,j) + _item[4*i + j];
		return *this;
	}

	Matrix4by4f & operator -= (Matrix4by4f const & mat)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				_item[4*i + j] -= mat(i,j);
		return *this;
	}

	Matrix4by4f getInverse()
	{
		double inv[16], det;
		int i,j;
	

	    inv[0] = _item[5]  * _item[10] * _item[15] - 
		     _item[5]  * _item[11] * _item[14] - 
		     _item[9]  * _item[6]  * _item[15] + 
		     _item[9]  * _item[7]  * _item[14] +
		     _item[13] * _item[6]  * _item[11] - 
		     _item[13] * _item[7]  * _item[10];

	    inv[4] = -_item[4]  * _item[10] * _item[15] + 
		      _item[4]  * _item[11] * _item[14] + 
		      _item[8]  * _item[6]  * _item[15] - 
		      _item[8]  * _item[7]  * _item[14] - 
		      _item[12] * _item[6]  * _item[11] + 
		      _item[12] * _item[7]  * _item[10];

	    inv[8] = _item[4]  * _item[9] * _item[15] - 
		     _item[4]  * _item[11] * _item[13] - 
		     _item[8]  * _item[5] * _item[15] + 
		     _item[8]  * _item[7] * _item[13] + 
		     _item[12] * _item[5] * _item[11] - 
		     _item[12] * _item[7] * _item[9];

	    inv[12] = -_item[4]  * _item[9] * _item[14] + 
		       _item[4]  * _item[10] * _item[13] +
		       _item[8]  * _item[5] * _item[14] - 
		       _item[8]  * _item[6] * _item[13] - 
		       _item[12] * _item[5] * _item[10] + 
		       _item[12] * _item[6] * _item[9];

	    inv[1] = -_item[1]  * _item[10] * _item[15] + 
		      _item[1]  * _item[11] * _item[14] + 
		      _item[9]  * _item[2] * _item[15] - 
		      _item[9]  * _item[3] * _item[14] - 
		      _item[13] * _item[2] * _item[11] + 
		      _item[13] * _item[3] * _item[10];

	    inv[5] = _item[0]  * _item[10] * _item[15] - 
		     _item[0]  * _item[11] * _item[14] - 
		     _item[8]  * _item[2] * _item[15] + 
		     _item[8]  * _item[3] * _item[14] + 
		     _item[12] * _item[2] * _item[11] - 
		     _item[12] * _item[3] * _item[10];

	    inv[9] = -_item[0]  * _item[9] * _item[15] + 
		      _item[0]  * _item[11] * _item[13] + 
		      _item[8]  * _item[1] * _item[15] - 
		      _item[8]  * _item[3] * _item[13] - 
		      _item[12] * _item[1] * _item[11] + 
		      _item[12] * _item[3] * _item[9];

	    inv[13] = _item[0]  * _item[9] * _item[14] - 
		      _item[0]  * _item[10] * _item[13] - 
		      _item[8]  * _item[1] * _item[14] + 
		      _item[8]  * _item[2] * _item[13] + 
		      _item[12] * _item[1] * _item[10] - 
		      _item[12] * _item[2] * _item[9];

	    inv[2] = _item[1]  * _item[6] * _item[15] - 
		     _item[1]  * _item[7] * _item[14] - 
		     _item[5]  * _item[2] * _item[15] + 
		     _item[5]  * _item[3] * _item[14] + 
		     _item[13] * _item[2] * _item[7] - 
		     _item[13] * _item[3] * _item[6];

	    inv[6] = -_item[0]  * _item[6] * _item[15] + 
		      _item[0]  * _item[7] * _item[14] + 
		      _item[4]  * _item[2] * _item[15] - 
		      _item[4]  * _item[3] * _item[14] - 
		      _item[12] * _item[2] * _item[7] + 
		      _item[12] * _item[3] * _item[6];

	    inv[10] = _item[0]  * _item[5] * _item[15] - 
		      _item[0]  * _item[7] * _item[13] - 
		      _item[4]  * _item[1] * _item[15] + 
		      _item[4]  * _item[3] * _item[13] + 
		      _item[12] * _item[1] * _item[7] - 
		      _item[12] * _item[3] * _item[5];

	    inv[14] = -_item[0]  * _item[5] * _item[14] + 
		       _item[0]  * _item[6] * _item[13] + 
		       _item[4]  * _item[1] * _item[14] - 
		       _item[4]  * _item[2] * _item[13] - 
		       _item[12] * _item[1] * _item[6] + 
		       _item[12] * _item[2] * _item[5];

	    inv[3] = -_item[1] * _item[6] * _item[11] + 
		      _item[1] * _item[7] * _item[10] + 
		      _item[5] * _item[2] * _item[11] - 
		      _item[5] * _item[3] * _item[10] - 
		      _item[9] * _item[2] * _item[7] + 
		      _item[9] * _item[3] * _item[6];

	    inv[7] = _item[0] * _item[6] * _item[11] - 
		     _item[0] * _item[7] * _item[10] - 
		     _item[4] * _item[2] * _item[11] + 
		     _item[4] * _item[3] * _item[10] + 
		     _item[8] * _item[2] * _item[7] - 
		     _item[8] * _item[3] * _item[6];

	    inv[11] = -_item[0] * _item[5] * _item[11] + 
		       _item[0] * _item[7] * _item[9] + 
		       _item[4] * _item[1] * _item[11] - 
		       _item[4] * _item[3] * _item[9] - 
		       _item[8] * _item[1] * _item[7] + 
		       _item[8] * _item[3] * _item[5];

	    inv[15] = _item[0] * _item[5] * _item[10] - 
		      _item[0] * _item[6] * _item[9] - 
		      _item[4] * _item[1] * _item[10] + 
		      _item[4] * _item[2] * _item[9] + 
		      _item[8] * _item[1] * _item[6] - 
		      _item[8] * _item[2] * _item[5];

	    det = _item[0] * inv[0] + _item[1] * inv[4] + _item[2] * inv[8] + _item[3] * inv[12];

	    if (det == 0)
		cout << "det = 0";

	    det = 1.0 / det;

	    Matrix4by4f inverseMat = Matrix4by4f(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	    for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			inverseMat(i,j) = inv[i*4+j] * det;

	    return inverseMat;
	}

	string toString(){
	    string matrixString = "";
 
	    for(int row=0;row<4;row++){
		for(int column=0;column<4;column++){
		    float matrixElement = _item[4*row+column];
		    char* matrixElementChar = new char[32];
		    sprintf(matrixElementChar,"%.3f",matrixElement);
		    matrixString += matrixElementChar;
		    matrixString += "\t";
		}
	 
		matrixString += "\n";
	    }
 
	    return matrixString;
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

void matDotVec(Matrix4by4f m, Vector3f * v)
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
}

class Bone {
	private:
		Vector3f position;
		Vector3f rotation;
		Matrix4by4f rotationMatrix;
		Matrix4by4f translationMatrix;
		Matrix4by4f transformMatrix;
		Matrix4by4f restTransformMatrix;
		Matrix4by4f inverseRestTransformMatrix;
	public:
		int thisBoneId, prevBoneId;
		Bone(float x, float y, float z) {position[0] = x; position[1] = y; position[2] = z; thisBoneId = 0, prevBoneId = 0;}
		Bone(float x, float y, float z, float r1, float r2, float r3) {Bone(x,y,z); rotation[0] = r1; rotation[1] = r2; rotation[2] = r3; }
		Bone(int thisBone, int prevBone, float x, float y, float z, float r1, float r2, float r3){
			thisBoneId = thisBone, prevBoneId = prevBone;
			position[0] = x;
			position[1] = y;
			position[2] = z;
			rotation[0] = 0;
			rotation[1] = 0;
			rotation[2] = 0;}
		Vector3f& getRestPosition() {return position;}
		Vector3f& getRestRotation() {return rotation;}
		Matrix4by4f& getTranslationMatrix() {return translationMatrix;}
		Matrix4by4f& getRotationMatrix() {return rotationMatrix;}
		Matrix4by4f& getRestTransformMatrix() {return restTransformMatrix;}
		Matrix4by4f& getTransformMatrix() {return transformMatrix;}
		Matrix4by4f& getInverseRestTransformMatrix() {return inverseRestTransformMatrix;}
};


class Skeleton {
	private:	
		vector<Bone> bones;
	public:
		void loadFile(char* filename);
		Bone& getBone(int i){if (bones.size() > i) return bones[i];}
		vector<Bone>& getBones(){return bones;}
		void addBone(Bone &bone) {bones.push_back(bone);}
		void computeBoneTransforms();
};

struct VertexWeights {
	vector<float> weights;
	int vertexId;
};

#endif //_rt_H
