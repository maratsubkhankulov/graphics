#include "teapot.h"

#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	float R,G,B;
	
	Light()
	{R = 1; G = 1; B = 1;};	

	Light(float r, float g, float b){
		R = r; G = g; B = b;
	};
};

class AmbientLight:public Light
{
public:	
	AmbientLight():Light(){};	
		
	AmbientLight(float r, float g, float b):Light(r,g,b)
	{};

	AmbientLight operator = (AmbientLight obj) 
	{
		R = obj.R; G = obj.G; B = obj.B;
	}
};

class DirectionalLight:public Light
{
public:
	Vector3f direction;
	Vector3f position;

	DirectionalLight(float r, float g, float b, Vector3f pos, Vector3f dir):Light(r,g,b){
		direction = dir; direction.normalize();
		position = pos; position.normalize();
	};

	DirectionalLight(float r, float g, float b, Vector3f dir){
		DirectionalLight(r,g,b,Vector3f(10.0f,10.0f,10.0f),dir);
	};

	DirectionalLight(){DirectionalLight(1.0f,1.0f,1.0f,Vector3f(1.0f,1.0f,1.0f));};

	DirectionalLight operator = (DirectionalLight obj) 
	{
		R = obj.R; G = obj.G; B = obj.B; direction = obj.direction;
	};
};

/*class DiffuseLight:public Light
{
public:
	Vector3f direction;

	DiffuseLight(float r, float g, float b, Vector3f dir):Light(r,g,b){
		direction = dir;
	}
};

class SpecularLight: public DiffuseLight
{
public: 
	SpecularLight(float r, float g, float b, Vector3f dir):DiffuseLight(r,g,b,dir){
	}
};*/

#endif
			


		
