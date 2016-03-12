#include "teapot.h"

#ifndef CAMERA_H
#define CAMERA_H

const int screenWidth = 800;
const int screenHeight = 640;

float zBuffer[screenWidth][screenHeight];

/*Set up camera*/
Vector3f c = Vector3f(0,0,-920); //Camera position
Vector3f lookAt = Vector3f(0,0,1); //Focus point
Vector3f r = Vector3f(0,0,0); //Camera rotation
Vector3f e = Vector3f(screenWidth/2,screenHeight/2,1.0/tan(75/2.0)); //Camera position relative to projection plane
float fov = 2.0;
float near = -1.0;
float far = 1.0;

void initZBuffer()
{
	for (int x = 0; x < screenWidth; x++)
		for (int y = 0; y < screenHeight; y++)
			zBuffer[x][y] = far;
}

void projectPerspective(Vector4f *v)
{	
	/* Set up Camera transform matrices */

	Matrix4by4f cameraTranslate = Matrix4by4f(	1,0,0,c[0],
					 		0,1,0,c[1],
					 		0,0,1,c[2],
					 		0,0,0,1);

	Matrix4by4f cameraRotateX = Matrix4by4f(	1,0,0,0,
					 		0,cos(r[0]),-sin(r[0]),0,
					 		0,sin(r[0]), cos(r[0]),0,
					 		0,0,0,1);

	Matrix4by4f cameraRotateY = Matrix4by4f(	cos(r[1]),0,sin(r[1]),0,
					 		0,1,0,0,
					 		-sin(r[1]),0,cos(r[1]),0,
					 		0,0,0,1);

	Matrix4by4f cameraRotateZ = Matrix4by4f(	cos(r[2]),-sin(r[2]),0,0,
					 		sin(r[2]),cos(r[2]),0,0,
					 		0,0,1,0,
					 		0,0,0,1);

	/* Set up projection matrix */
	Matrix4by4f projection = Matrix4by4f(	1,0,0,-e[0],
					 	0,1,0,-e[1],
					 	0,0,1,0,
					 	0,0,1/e[2],1);
	//matDotVec(cameraTranslate, v);
	//Matrix4by4f project;

	/*float fov, angle;
	//Calculate Field of View
	angle = 75;
	fov = 1.0*tan(angle/2.0);*/
	
	/*Calculate the camera transform*/
	matDotVec(cameraTranslate, v);
	matDotVec(cameraRotateX, v);
	matDotVec(cameraRotateY, v);
	matDotVec(cameraRotateZ, v);
	
	/*Project onto 2d plane*/
	(*v)[3] = 1;
	matDotVec(projection, v);

	/*Divide by w/Homogenize/What's it called?*/
	(*v)[0] = ((*v)[0] * screenWidth) / (fov * (*v)[3]) + screenWidth/2;
	(*v)[1] = ((*v)[1] * screenHeight) / (fov * (*v)[3]) + screenHeight/2;	
}

#endif
