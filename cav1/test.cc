// Testing space for functions and classes

#include <math.h>
#include <iostream>
#include <fstream>
#include "view.h"

int main(int argc, char** argv){

	Matrix4by4f id = Matrix4by4f(	1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1);

	Matrix4by4f m1 = Matrix4by4f(	1,0,0,5,
					0,1,0,4,
					0,0,1,3,
					0,0,0,1);


	Matrix4by4f m2 = Matrix4by4f(	1,0,0,6,
					0,1,0,5,
					0,0,1,4,
					0,0,0,1);

	float a = M_PI;

	Matrix4by4f r1 = Matrix4by4f(	sin(a),-cos(a),0,0,
					cos(a),sin(a),0,0,
					0,0,1,0,
					0,0,0,1);

	//cout << id.toString() << endl;
	//cout << m1.toString() << endl; 
	//cout << m2.toString() << endl;
	//cout << r1.toString() << endl;

	//Tests
	Matrix4by4f p1 = id;
	Matrix4by4f p2 = r1*id;
	Matrix4by4f p3 = m1*m2;
	Matrix4by4f p4 = r1*id*m1*m2;
	cout << "p1" << endl << p1.toString() << endl; 
	cout << "p2" << endl << p2.toString() << endl;
	cout << "p3" << endl << p3.toString() << endl;
	cout << "p4" << endl << p4.toString() << endl;
	
}
