#include <iostream>
#include <fstream>
#include <cstring>
#include <GL/glut.h>
#include "teapot.h"
#include "constants.h"
#include "math.h"
#include "object.h"
#include "linedrawing.h"
#include "camera.h"
#include "renderingoptions.h"
#include "utilities.h"
#include "light.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "imageloader.h"

#define USE_MATH_DEFINES

using namespace std;

/*Colors & Lights*/
float lineR = 0.5, lineG = 1, lineB = 1;
float bgR = 0, bgG=0, bgB = 0;
AmbientLight ambient1;// = AmbientLight(1.0f,1.0f,1.0f);
DirectionalLight dirLight1;// = DirectionalLight(1.0f,1.0f,1.0f,Vector3f(1.0f,1.0f,1.0f));

/*Line modes*/
int LINE_MODE = 2;
const int SLOPE = 0;
const int DDA = 1;
const int MIDPOINT = 2;
const int GUPTA = 3;

TriangleMesh trig;
vector<Vector3f> trigNormals;
vector<Vector3f> vertexNormals; 

/*Texture*/
Image* image;

void initRendering(char * filename) {
	image = loadBMP(filename);
}

void TriangleMesh::loadFile(char * filename)
{
	ifstream f(filename);


	if (f == NULL) {
		cerr << "failed reading polygon data file " << filename << endl;
		exit(1);
	}

	char buf[1024];
	char header[100];
	float x,y,z;
	float xmax,ymax,zmax,xmin,ymin,zmin;
	int v1, v2, v3, n1, n2, n3;

	xmax =-10000; ymax =-10000; zmax =-10000;
	xmin =10000; ymin =10000; zmin =10000;
	Vector3f av;
	av[0] = av[1] = av[2] = 0.f;

	while (!f.eof()) {
		    f.getline(buf, sizeof(buf));
		    sscanf(buf, "%s", header);  

		    if (strcmp(header, "v") == 0) {
			sscanf(buf, "%s %f %f %f", header, &x, &y, &z);  

		//	x *= 1000; y *= 1000; z *= 1000;

			_v.push_back(Vector4f(x,y,z,1));

			av[0] += x; av[1] += y; av[2] += z;

			if (x > xmax) xmax = x;
			if (y > ymax) ymax = y;
			if (z > zmax) zmax = z;

			if (x < xmin) xmin = x;
			if (y < ymin) ymin = y;
			if (z < zmin) zmin = z;
		    }
		    else if (strcmp(header, "f") == 0) {
			sscanf(buf, "%s %d %d %d", header, &v1, &v2, &v3);
			
			Triangle trig(v1-1, v2-1, v3-1);
			_trig.push_back(trig);

		    }
 	}

	_xmin = xmin; _ymin = ymin; _zmin = zmin;
	_xmax = xmax; _ymax = ymax; _zmax = zmax;

	float range; 
	if (xmax-xmin > ymax-ymin) range = xmax-xmin;
	else range = ymax-ymin;

	for (int j = 0; j < 3; j++) av[j] /= _v.size();

	for (int i = 0; i < _v.size(); i++) 
	{
		for (int j = 0; j < 3; j++) _v[i][j] = (_v[i][j]-av[j])/range*400;  
	}
	cout << "trig " << _trig.size() << " vertices " << _v.size() << endl;
	f.close();
};

void initScene()
{
	LINE_MODE = MIDPOINT;

	initZBuffer();

	c[0] = 0, c[1] = -250, c[2] = -920; //Camera position
	r[0] = 0.3, r[1] = 0, r[2] = 0; //Camera rotation
	e[0] = 0, e[1] = 0, e[2] = 1.0/tan(75/2.0); //Camera position relative to projection plane
	fov = 2.0;
	
	initRendering("rock.bmp");
	
	objectAngle = 0;
	objectScale = 1;
	objectShear = 0;
	objectPosition[0] = 0, objectPosition[1] = 0, objectPosition[2] = 0, objectPosition[3] = 1;

	ambient1 = AmbientLight(0.2f,0.3f,0.4f);
	dirLight1 = DirectionalLight(1.0f,1.0f,1.0f,Vector3f(1.0f,0.0f,0.0f),Vector3f(0.0f,0.0f,1.0f));

	//calculateNormals for the teapot
		//Find normals of all triangles
		Vector3f v1, v2, v3;
		Vector4f pv1, pv2, pv3;
		float a1,b1,c1, a2, b2, c2, a3, b3, c3;
		for (int i = 0; i < trig.trigNum(); i++)
		{
			trig.getTriangleVertices(i,pv1,pv2,pv3);
			a1 = pv1[0]; b1 = pv1[1]; c1 = pv1[2]; //v1 = 
			a2 = pv2[0]; b2 = pv2[1]; c2 = pv2[2]; //v2 = Vector3f(a,b,c); 
			a3 = pv3[0]; b3 = pv3[1]; c3 = pv3[2]; //v3 = Vector3f(a,b,c); 
			trigNormals.push_back(getPlaneNormal(Vector3f(a1,b1,c1), Vector3f(a2,b2,c2), Vector3f(a3,b3,c3)));
		}
		
		//Find normals for all vertices
		Triangle tri = Triangle(0,0,0);
		Vector4f cv;
		for (int n = 0; n < trig.getVertexNum()-1; n++) // LAST VERTEX NORMAL NOT DEFINED, WHY?
		{
			Vector3f cumulativeNormal = Vector3f(0,0,0);
			cv = *(trig.getVertex(n));
			int normalCounter = 0;			
			for (int i = 0; i < trig.trigNum(); i++)
			{
				trig.getTriangleVertices(i,pv1,pv2,pv3);
				tri = trig.getTriangle(i);
				if (areSimilarVector4f(pv1,cv) || areSimilarVector4f(pv2, cv) || areSimilarVector4f(pv3, cv)){
					normalCounter++;
					cumulativeNormal += trigNormals[i];
				}
			}
			cumulativeNormal/=normalCounter;
			cumulativeNormal.normalize();
			vertexNormals.push_back(cumulativeNormal);

		}
		/*for (int n = 0; n < trig.getVertexNum()-1; n++) // LAST VERTEX NORMAL NOT DEFINED, WHY?
		{
			Vector3f cumulativeNormal = Vector3f(0,0,0);
			vector<int> listOfTriangleIndeces;
			
			for (int i = 0; i < trig.trigNum(); i++)
			{
				tri = trig.getTriangle(i);
				if (tri.getVertexIndex(0) == n || tri.getVertexIndex(1) == n || tri.getVertexIndex(2) == n){
					listOfTriangleIndeces.push_back(i);
				}
			}
			for (int i = 0; i < listOfTriangleIndeces.size(); i++) {
				cumulativeNormal += trigNormals[listOfTriangleIndeces[i]];
			}
			cumulativeNormal/=listOfTriangleIndeces.size();
			cumulativeNormal.normalize();
			vertexNormals.push_back(cumulativeNormal);
		}	*/				
}

void processArrowKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			rotateObject(0.1);			
			break;
		case GLUT_KEY_RIGHT:
			rotateObject(-0.1);			
			break;
		case GLUT_KEY_UP:
			scaleObject(0.1);
			break;
		case GLUT_KEY_DOWN:
			scaleObject(-0.1);
			break;
	}
	glutPostRedisplay();
}

void processKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case KEY_W:
			c[2] = c[2]+5;
			break;
		case KEY_S:
			c[2] = c[2]-5;
			break;
		case KEY_A:
			r[1] += 0.01;
			break;
		case KEY_D:
			r[1] -= 0.01;
			break;
		case '1':
			LINE_MODE = SLOPE;
			break;
		case '2':
			LINE_MODE = DDA;
			break;
		case '3':
			LINE_MODE = MIDPOINT;
			break;
		case '4':
			LINE_MODE = GUPTA;
			break;
		case '5':
			objectPosition[0]+=1;
			break;
		case '6':
			objectShear-=0.01;
			break;
		case '0':
			initScene();
			break;
		case '=':
			fov-= 0.01;
			break;
		case '-':
			fov+= 0.01;
			break;
	}
	glutPostRedisplay();
}

bool updateZBuffer(int x, int y, float z)
{
	float depth = getZDepth(z,near,far);
	if (0 <= x && x <screenWidth && 0<= y && y < screenHeight)	
		if (depth < zBuffer[x][y]){
			zBuffer[x][y] = depth;
			return true;
		}
	return false;
}

void processMouse(int _x, int _y)
{
	static int x = _x;//screenWidth / 2;
	static int y = _y;//screenHeight / 2;

	int dx = x - _x;
	int dy = y - _y;
	x = _x;
	y = _y;

	r[1] -= dx*0.002;
	r[0] -= dy*0.002;

	int margin = screenWidth/4;
	/*if (x < margin || x > screenWidth-margin || y < margin || y > screenHeight-margin)
	{
		x = screenWidth/2;
		y = screenHeight/2;
		glutWarpPointer(screenWidth / 2, screenHeight / 2);
	}*/
	glutPostRedisplay();
}

void shadePixel(int x, int y)
{	
	glColor3f(ambient1.R*objectR, ambient1.G*objectG, ambient1.B*objectB);	
	glVertex2i(x, y);
}

void shadePixelSingleLight(int x, int y, AmbientLight amb, DirectionalLight dir, Vector3f normal, Vector3f objectColor, Vector3f objectRoughness, Vector3f objectSpecularity, Vector3f viewerVector, Vector3f reflectionVector){
	//For each R,G and B decide the intensity
	float R = 0,G = 0,B = 0;
	float n = 100;
	//cout << "specularFactor " << specularFactor << "\n";
	R = ambient1.R*objectColor[0] + dir.R*(objectRoughness[0]*(normal*dir.direction) + objectSpecularity[0]*pow((viewerVector*reflectionVector),n));
	G = ambient1.G*objectColor[1] + dir.G*(objectRoughness[1]*(normal*dir.direction) + objectSpecularity[1]*pow((viewerVector*reflectionVector),n));
	B = ambient1.B*objectColor[2] + dir.B*(objectRoughness[2]*(normal*dir.direction) + objectSpecularity[2]*pow((viewerVector*reflectionVector),n));
	static int counter = 50;
	glColor3f(R,G,B);		
	glVertex2i(x, y);
}

void shadePixelSingleOmniLight(int x, int y, AmbientLight amb, Vector3f dir, Vector3f normal, Vector3f objectColor, Vector3f objectRoughness, Vector3f objectSpecularity, Vector3f viewerVector, Vector3f reflectionVector){
	//For each R,G and B decide the intensity
	float R = 0,G = 0,B = 0;
	float n = 100;
	//cout << "specularFactor " << specularFactor << "\n";
	R = ambient1.R*objectColor[0] + 0.5*(objectRoughness[0]*(normal*dir) + objectSpecularity[0]*pow((viewerVector*reflectionVector),n));
	G = ambient1.G*objectColor[1] + 0.5*(objectRoughness[1]*(normal*dir) + objectSpecularity[1]*pow((viewerVector*reflectionVector),n));
	B = ambient1.B*objectColor[2] + 0.5*(objectRoughness[2]*(normal*dir) + objectSpecularity[2]*pow((viewerVector*reflectionVector),n));
	glColor3f(R,G,B);		
	glVertex2i(x, y);
}

void drawScene()
{
	int trignum = trig.trigNum();
	int vertexnum = trig.getVertexNum();
	initZBuffer();
	Vector4f pv1,pv2,pv3; // Homogenous vectors for homogenous transformations
	
	TriangleMesh transTrig = trig;
/* Geometry */	

//Model transform
	Matrix4by4f rotationMatrix = Matrix4by4f(cos(objectAngle),0, sin(objectAngle),0,
						 0,1,0,0,
						 -sin(objectAngle),0,cos(objectAngle),0,
						 0,0,0,1);

	Matrix4by4f scaleMatrix = Matrix4by4f(	 1,0,0,0,
						 0,objectScale,0,0,
						 0,0,1,0,
						 0,0,0,1);

	Matrix4by4f translationMatrix = Matrix4by4f(1,0,0,objectPosition[0],
						0,1,0,objectPosition[1],
						0,0,1,objectPosition[2],
						0,0,0,1);

	Matrix4by4f shearMatrix = Matrix4by4f(1,objectShear,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1);
	for (int i = 0; i < vertexnum; i++)
	{		
		matDotVec(translationMatrix, transTrig.getVertex(i));
		matDotVec(rotationMatrix, transTrig.getVertex(i));
		matDotVec(scaleMatrix, transTrig.getVertex(i));
		matDotVec(shearMatrix, transTrig.getVertex(i));

//View Transform

		projectPerspective(transTrig.getVertex(i));
	}

	//Light Transform
	/*matDotVec(translationMatrix, &dirLight1.direction);
	matDotVec(rotationMatrix, &dirLight1.direction);
	matDotVec(scaleMatrix, &dirLight1.direction);
	matDotVec(shearMatrix, &dirLight1.direction);*/

//Hidden Surface removal

//Shading: reflection and lighting

	
/* Rasterization */
	glBegin(GL_POINTS);
	int boxX0, boxY0, boxX1, boxY1 = 0;

//Rasterization and sampling

//Texture mapping

//Image composition

//Intensity and color quantization
	
//Barycentric rasterisation
	if (bBarycentric) {	
		int transTrignum = transTrig.trigNum();
		float alpha, beta, gamma = 0;
		Vector3f normal = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f view = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f reflection = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f objectColor = Vector3f(objectR, objectG, objectB);
		Vector3f objectRoughness = Vector3f(0.5f, 0.5f, 0.6f);
		Vector3f objectSpecularity = Vector3f(0.6f, 0.3f, 0.7f);
		Vector3f lightDirection = Vector3f(0,0,0);
		Vector3f point = Vector3f(0,0,0);
		//For Phong
		Triangle tri = Triangle(0,0,0);
		int ia, ib, ic;
		for (int i = 0; i < transTrignum; i++)
		{
			transTrig.getTriangleVertices(i,pv1,pv2,pv3);
		//Calculate bounding box
			boxX0 = floor(min(pv1[0], pv2[0], pv3[0]));
			boxY0 = floor(min(pv1[1], pv2[1], pv3[1]));
			boxX1 = ceil(max(pv1[0], pv2[0], pv3[0]));
			boxY1 = ceil(max(pv1[1], pv2[1], pv3[1]));	

		//For each pixel in bounding box calculate barycentric coodrinates
			for (int x = boxX0; x < boxX1; x++){
				for (int y = boxY0; y < boxY1; y++) {
					if (isBarycentric(x,y,pv1[0],pv1[1],pv2[0],pv2[1],pv3[0],pv3[1])||false){
						switch(SHADING_MODE){
							case (BASELINE):							
								shadePixel(x, y);
								break;
							case (FLAT):
								//Find reflection vector
								reflectVectorGivenNormal(dirLight1.direction, trigNormals[i], &reflection);
								//cout << "Reflection vector: " << view[0] << "," << view[1] << "," << view[2] << "\n";
								//Find viewpoint
								view[0]=x,view[1]=y,view[2] = e[2]; //Where e camera position relative to projection plane
								view -= c; //c - camera position
								view.normalize();			
								if (updateZBuffer(x,y,getZOnPlane(x,y,pv1, pv2, pv3)))					
									shadePixelSingleLight(x,y, ambient1, dirLight1, trigNormals[i], objectColor, objectRoughness, objectSpecularity, view, reflection);
								break;
							case (PHONG_1):
								//Interpolate to find normal at pixel
								tri = transTrig.getTriangle(i);								
								ia = tri.getVertexIndex(0); ib = tri.getVertexIndex(1); ic = tri.getVertexIndex(2);
								interpolateNormals(x,y,pv1[0],pv1[1],pv2[0],pv2[1],pv3[0],pv3[1],
										vertexNormals[ia],vertexNormals[ib], vertexNormals[ic], &normal);
								
								//Find reflection vector
								reflectVectorGivenNormal(dirLight1.direction, normal, &reflection);
								//cout << "Reflection vector: " << view[0] << "," << view[1] << "," << view[2] << "\n";
								//Find viewpoint
								view[0]=x,view[1]=y,view[2] = e[2]; //Where e camera position relative to projection plane
								view -= c; //c - camera position
								view.normalize();
								if (updateZBuffer(x,y,getZOnPlane(x,y,pv1, pv2, pv3)))					
									shadePixelSingleLight(x,y, ambient1, dirLight1, normal, objectColor, objectRoughness, objectSpecularity, view, reflection);
								break;
							case (PHONG_2):
								//Find light direction to the point
								findPointInTriangle(x,y,pv1[0],pv1[1],pv1[2],pv2[0],pv2[1],pv2[2],pv3[0],pv3[1],pv3[2],&point);
								lightDirection = point -= dirLight1.position;
								lightDirection.normalize();
								//Interpolate to find normal at pixel
								tri = transTrig.getTriangle(i);								
								ia = tri.getVertexIndex(0); ib = tri.getVertexIndex(1); ic = tri.getVertexIndex(2);
								interpolateNormals(x,y,pv1[0],pv1[1],pv2[0],pv2[1],pv3[0],pv3[1],
										vertexNormals[ia],vertexNormals[ib], vertexNormals[ic], &normal);
								
								//Find reflection vector
								reflectVectorGivenNormal(lightDirection, normal, &reflection);
								//cout << "Reflection vector: " << view[0] << "," << view[1] << "," << view[2] << "\n";
								//Find viewpoint
								view[0]=x,view[1]=y,view[2] = e[2]; //Where e camera position relative to projection plane
								view -= c; //c - camera position
								view.normalize();
								if (updateZBuffer(x,y,getZOnPlane(x,y,pv1, pv2, pv3)))					
									shadePixelSingleOmniLight(x,y, ambient1, lightDirection, normal, objectColor, objectRoughness, objectSpecularity, view, reflection);
								break;
							case (TEXTURE):
								//Interpolate to find normal at pixel
								tri = transTrig.getTriangle(i);								
								ia = tri.getVertexIndex(0); ib = tri.getVertexIndex(1); ic = tri.getVertexIndex(2);
								interpolateNormals(x,y,pv1[0],pv1[1],pv2[0],pv2[1],pv3[0],pv3[1],
										vertexNormals[ia],vertexNormals[ib], vertexNormals[ic], &normal);
								
								//Find reflection vector
								reflectVectorGivenNormal(dirLight1.direction, normal, &reflection);
								//cout << "Reflection vector: " << view[0] << "," << view[1] << "," << view[2] << "\n";
								//Find viewpoint
								view[0]=x,view[1]=y,view[2] = e[2]; //Where e camera position relative to projection plane
								view -= c; //c - camera position
								view.normalize();
								//Get colour at corresponding position in the texture
								float r,g,b;
								image->getPixel(x,y,&r,&g,&b);//getPixel(x,y,&r,&g,&b);
								Vector3f texColor(r,g,b);
								if (updateZBuffer(x,y,getZOnPlane(x,y,pv1, pv2, pv3)))					
									shadePixelSingleLight(x,y, ambient1, dirLight1, normal, 
									texColor, objectRoughness, objectSpecularity, view, reflection);
								break;
								
						}
					}
				}
			}
		}
	}
	//Scanline algorithm
	if (bScanline) {
		/*vector <Vector3f> intersectionList;
		int transTrignum = transTrig.trigNum();
		int x0, x1 = 0;
		for (int i = 0; i < transTrignum; i++)
		{
			transTrig.getTriangleVertices(i,pv1,pv2,pv3);
			//Calculate bounding box
			boxX0 = min(pv1[0], pv2[0], pv3[0]);
			boxY0 = min(pv1[1], pv2[1], pv3[1]);
			boxX1 = max(pv1[0], pv2[0], pv3[0]);
			boxY1 = max(pv1[1], pv2[1], pv3[1]);
		//Find intersections
			for (int y = boxY0; y < boxY1; y++) {
				if (getIntersectionPointsTrigWithScanline(pv1[0], pv1[1], pv2[0], pv2[1], pv3[0], pv3[1], y, &x0, &x1))
				{
					intersectionList.push_back(Vector4f(x0, x1, y));
				}
			}
		//Sort intersections by increasing x
			sort(&intersectionList);
		//Fill in all pixels between pairs of intersections
			int x, y = 0;
			for (int i = 0; i < intersectionList.size; i++) {
				for (int x = intersectionList[i][0]; x < intersectionList[i][1]; x++) shadePixel(x, intersectionList[i][2]);
			}
		}*/
	}				
		
//Wireframe
	if (bWireframe) {
		glColor3f(0.5,0.7,0.5);		
		for (int i = 0 ; i < trignum; i++) {	
			transTrig.getTriangleVertices(i,pv1,pv2,pv3);
			drawMidpointLine(pv1[0],pv1[1],pv2[0],pv2[1]);
			drawMidpointLine(pv2[0],pv2[1],pv3[0],pv3[1]);
			drawMidpointLine(pv3[0],pv3[1],pv1[0],pv1[1]);
		}
	}
	if (bNormals) {
		float x,y,z;
		glColor3f(1,1,1);
		int index = 0;
		for (int i = 0 ; i < trig.getVertexNum()-1; i++) {	
			cout << vertexNormals[0] << "," << vertexNormals[1] << "," << vertexNormals[2] << "\n";
			x = (*trig.getVertex(i))[0], y = (*trig.getVertex(i))[1], z = (*trig.getVertex(i))[2];
			Vector4f p1 = Vector4f(x,y,z,1);
			Vector4f p2 = Vector4f(x+(vertexNormals[index])[0]*20,y+(vertexNormals[index])[1]*20,z+(vertexNormals[index])[2]*20,1);
			matDotVec(translationMatrix, &p1);
			matDotVec(rotationMatrix, &p1);
			matDotVec(scaleMatrix, &p1);
			matDotVec(shearMatrix, &p1);
			projectPerspective(&p1);
			matDotVec(translationMatrix, &p2);
			matDotVec(rotationMatrix, &p2);
			matDotVec(scaleMatrix, &p2);
			matDotVec(shearMatrix, &p2);
			projectPerspective(&p2);
			cout << "done translating normal\n";
			drawMidpointLine(p1[0],p1[1],p2[0], p2[1]);
			cout << "done drawing normal\n";
			index++;
		}
	}
	/*for (int i = 0 ; i < trig.getVertexNum(); i++) {
		
	}*/
	glEnd();

}

void drawTriangle() {
	int Ax = 0;
	int Ay = 0;
	int Bx = 100;
	int By = 100;
	int Cx = 100;
	int Cy = 0;

	for (int i = Ax; i < Cx; i++)
		for (int j = Ay; j<By; j++) {
			if (isBarycentric(i, j, Ax, Ay, Bx, By, Cx, Cy))
			{
				glColor3f(1,1,1);
				glBegin(GL_POINTS);
				glVertex2i(i,j);
				glEnd();
			}
		}
	glFlush();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear OpenGL Window

	/*** clear the Zbuffer here ****/
	
	/*switch(LINE_MODE)
	{
		case SLOPE: drawPerspectiveSimple(drawSlopeInterceptLine); break;
		case DDA: drawPerspectiveSimple(drawDDALine); break;
		case MIDPOINT: drawPerspectiveSimple(drawMidpointLine); break;
		case GUPTA: drawPerspectiveSimple(drawGuptaSproullLine); break;
	}*/
	drawScene();
	//drawTriangle();
	glFlush();// Output everything
	//glutPostRedisplay();
}

int main(int argc, char **argv)
{
	if (argc >  1)  {
		trig.loadFile(argv[1]);
	}
	else {
		cerr << argv[0] << " <filename> " << endl;
		exit(1);
	}
	/* Test */
	/*Matrix4by4f camera = Matrix4by4f(1,0,0,0,
					 0,1,0,0,
					 0,0,1,0,
					 0,0,0,1);
	Vector4f v = Vector4f(1,1,1,1);
	cout << v;
	matDotVec(camera, &v);
	cout << v;
	int width, height;*/
	/* end test*/
	glutInit(&argc, argv);
	glutInitWindowSize(screenWidth, screenHeight);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutCreateWindow("SimpleExample");
	gluOrtho2D(0, screenWidth, 0,  screenHeight);
	glutDisplayFunc(myDisplay);// Callback function
	glutSpecialFunc(processArrowKeys); // Arrow Key controls
	glutKeyboardFunc(processKeyboard); // Letter Key controls
	glutPassiveMotionFunc(processMouse);
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
	initScene();
	glutMainLoop();// Display everything and wait
}

