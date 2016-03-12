#include <iostream>
#include <fstream>
#include <cstring>
#include "utilities.h"
#include "teapot.h"

using namespace std;

bool reflectionTest();

int main (int argc, char **argv)
{
	cout << "testing min max\n";	
	cout << min(1.0f,2.0f,3.0f) << "\n";
	cout << max(1.0f,2.0f,3.0f) << "\n";
	
	Vector3f v1 = Vector3f(2.0f,3.0f,5.0f);
	Vector3f v2 = Vector3f(5.0f,1.5f,5.5f);
	float dotProduct = v1*v2;
	cout << "testing dotproduct" << "\n";
	cout << dotProduct << "\n";

	v1 = v1.cross(v2);
	cout << "testing cross product" << "\n";
	cout << v1[0] << "," << v1[1] << "," << v1[2] << "\n";

	cout << "testing reflection vector calc \n";
	cout << (reflectionTest() == true ? "success" : "failure") << "\n";
	Vector3f ref = Vector3f(1,2,3);
	Vector3f dir = Vector3f(0.4,0.2,0.2);
	Vector3f normal = Vector3f(0.2,0.5,0.3);
	reflectVectorGivenNormal(dir, normal, &ref);
	cout << ref[0] << "," << ref[1] << "," << ref[2];
}

bool reflectionTest(){
	Vector3f ref = Vector3f(1,2,3);
	Vector3f dir = Vector3f(0.4,0.2,0.2);
	Vector3f normal = Vector3f(0.2,0.5,0.3);
	Vector3f rightAnswer = Vector3f(-0.304f, 0.04f, -0.056f);
	reflectVectorGivenNormal(dir, normal, &ref);
	return ref == rightAnswer;
}
