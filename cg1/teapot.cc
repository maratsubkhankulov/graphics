#include <iostream>
#include <fstream>
#include <cstring>
#include <GL/glut.h>
#include "teapot.h"
#include "constants.h"
#include "math.h"


#define USE_MATH_DEFINES

using namespace std;

int screenWidth = 800;
int screenHeight = 640; 

/*Set up camera*/
Vector3f c = Vector3f(0,0,-920); //Camera position
Vector3f lookAt = Vector3f(0,0,1); //Focus point
Vector3f r = Vector3f(0,0,0); //Camera rotation
Vector3f e = Vector3f(0,0,1.0/tan(75/2.0)); //Camera position relative to projection plane
float fov = 2.0;

/*Set up transform matrix*/
float objectAngle = 0;
float objectScale = 1;
float objectShear = 0;
Vector4f objectPosition = Vector4f(0,0,0,1);

/*Line modes*/
int LINE_MODE = 2;
const int SLOPE = 0;
const int DDA = 1;
const int MIDPOINT = 2;
const int GUPTA = 3;

/*Colors*/
float lineR = 0.5, lineG = 1, lineB = 1;
float bgR = 0, bgG=0, bgB = 0;

TriangleMesh trig;

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

			_v.push_back(Vector3f(x,y,z));


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


// Swap a and b
void swap(int* a,int* b)
{
	int t=*a;
	*a=*b;
	*b=t;
}


// Swap a and b
void swap(float* a,float* b)
{
	int t=*a;
	*a=*b;
	*b=t;
}

void drawGuptaPixel(int x, int y, int d, int dy, int dx, int swap)
{
	int offset = -1;
	int num = 0;
	float denom = 2*sqrt((float)(dx*dx+dy*dy));
	if (d<=0) { offset = -1; num = d + dx; } else { offset = 0; num = d - dx; }
	float strength[3];
	strength[0] = 1/(abs((2*dx-num)/denom)+1);
	strength[1] = 1/(abs((num)/denom)+1);
	strength[2] = 1/(abs((2*dx+num)/denom)+1);
	static int few = 300;
	if (few) {cout << " strength: " << strength[0] << " " << strength[1] << " " << strength[2] << "\n";few--;}
	for (int i = 0; i < 3; i++)
	{
		glColor3f(lineR*strength[i],lineG*strength[i],lineB*strength[i]);	
		//glColor3f(lineR,lineG,lineB);	
		if (swap)		
			glVertex2i(y+i+offset,x);
		else
			glVertex2i(x,y+i+offset);
	}
}

void drawGuptaSproullLine(float ix1, float iy1, float ix2, float iy2)
{	 
	int x1 = ix1, y1 = iy1, x2 = ix2, y2 = iy2, temp, swappedAxes = 0, yInc;
	int dx=abs(x1-x2);
	int dy=abs(y1-y2);
	
	//Swap axes
	if (dy>dx)
	{
		swap(&dy, &dx);
		swap(&x1, &y1);
		swap(&x2, &y2);
		swappedAxes = 1;
	}	
	//Choose p1 and p2 appropriately
	if (x1 > x2) 
	{
		swap(&x1, &x2);
		swap(&y1, &y2);
	}
	if (y1 > y2)
		yInc = -1;
	else
		yInc = 1;

	int x=(int)x1;
	int y=(int)y1;
	int d=2*dy-dx;
	int incrE=2*dy;
	int incrNE=2*(dy-dx);
	//if (swappedAxes)
		//glVertex2i(y, x);
	//else
		//glVertex2i(x, y);
	while (x < x2)
	{//walk through intermediate points
		if(d<=0)
			d+=incrE;
		else
		{
			d+=incrNE;
			y+=yInc;
		}
		x++;		
		//color the points
		drawGuptaPixel(x,y,d,dx,dy,swappedAxes);
	}	
}

void drawDDALine(float x1, float y1, float x2, float y2)
{
	float x,y;
	int dx = x2-x1, dy = y2-y1; // Optimise to INTEGER!!!
	int n = max(abs(dx),abs(dy)); // Range of t
	float dt = n, dxdt = dx/dt, dydt = dy/dt;
	x = x1;
	y = y1;
	while(n>0) {
		n--;		
		glVertex2i((int)x,(int)y);
		x += dxdt;
		y += dydt;
	}
}

void drawSlopeInterceptLine(float x1, float y1, float x2, float y2)
{
	float y = 0;
	float tmp = 0;
	if (x1 < x2) { tmp = x1, x1 = x2, x1 = tmp;}
	if (y1 < y2) { tmp = y1, y1 = y2, y1 = tmp;}
	float slope = (y1-y2)/(x1-x2);
	float b = y1-slope*x1;
	for (float x = x1; x <= x2; x++)
	{
 		y = (slope*x)+b;
		glVertex2i((int)x,(int)y);
	}
}

	//bool invert = false;
	/*if (dx == 0){
		invert = true;
	} else if (dy/dx > 1)
	{
		invert = true;
	}
	if (invert == true)
	{

	} else
	{	
		int x=(int)x1;
		int y=(int)y1;
	}*/

int sign(float n)
{
	if (n > 0) return 1;
	return -1;
}

void drawMidpointLine(float ix1, float iy1, float ix2, float iy2)
{	 
	int x1 = ix1, y1 = iy1, x2 = ix2, y2 = iy2, temp, swappedAxes = 0, yInc;
	int dx=abs(x1-x2);
	int dy=abs(y1-y2);
	
	//Swap axes
	if (dy>dx)
	{
		swap(&dy, &dx);
		swap(&x1, &y1);
		swap(&x2, &y2);
		swappedAxes = 1;
	}	
	//Choose p1 and p2 appropriately
	if (x1 > x2) 
	{
		swap(&x1, &x2);
		swap(&y1, &y2);
	}
	if (y1 > y2)
		yInc = -1;
	else
		yInc = 1;

	int x=(int)x1;
	int y=(int)y1;
	int d=2*dy-dx;
	int incrE=2*dy;
	int incrNE=2*(dy-dx);
	//if (swappedAxes)
		//glVertex2i(y, x);
	//else
		//glVertex2i(x, y);
	while (x < x2)
	{//walk through intermediate points
		if(d<=0)
			d+=incrE;
		else
		{
			d+=incrNE;
			y+=yInc;
		}
		x++;
		//color the points
		if(swappedAxes)
			glVertex2i(y,x);
		else
			glVertex2i(x,y);
	}
}

void drawOrthogonalSimple(void(*drawLineFunc)(float x1, float y1, float x2, float y2));
void drawPerspectiveSimple(void(*drawLineFunc)(float x1, float y1, float x2, float y2));

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear OpenGL Window

	/*** clear the Zbuffer here ****/
	
	switch(LINE_MODE)
	{
		case SLOPE: drawPerspectiveSimple(drawSlopeInterceptLine); break;
		case DDA: drawPerspectiveSimple(drawDDALine); break;
		case MIDPOINT: drawPerspectiveSimple(drawMidpointLine); break;
		case GUPTA: drawPerspectiveSimple(drawGuptaSproullLine); break;
	}
	// TEST AREA
	/*glBegin(GL_POINTS);
		//Draw Slope-intercept style
		//test 1 - p1 <-> p2
		drawGuptaSproullLine(-400, -100, 400, 100);
		drawGuptaSproullLine(400, 100, -400, -100);

		//test 2 - inverse gradient
		drawGuptaSproullLine(-400, 100, 400, -100);

		//test 3 - gradient > 1
		drawGuptaSproullLine(-100, -400, 100, 400);
		
		//test 4 - gradient < 0
		drawGuptaSproullLine(-100, 400, 100, -400);

		//test 5 - vertical line
		drawGuptaSproullLine(0, 400, 0, -400);*/
	glEnd();
	glFlush();// Output everything
	glutPostRedisplay();
}

void rotateObject(float n)
{
	objectAngle += n;
}

void scaleObject(float s)
{
	objectScale += s;
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
}

void initScene()
{
	LINE_MODE = MIDPOINT;

	//c[0] = 0, c[1] = 0, c[2] = -920; //Camera position
	//r[0] = 0, c[1] = 0, c[2] = 0; //Camera rotation
	e[0] = 0, e[1] = 0, e[2] = 1.0/tan(75/2.0); //Camera position relative to projection plane
	fov = 2.0;

	objectAngle = 0;
	objectScale = 1;
	objectShear = 0;
	objectPosition[0] = 0, objectPosition[1] = 0, objectPosition[2] = 0, objectPosition[3] = 1;
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
	gluOrtho2D(-screenWidth/2, screenWidth/2, -(float)screenHeight/2,  (float)screenHeight/2);
	glutDisplayFunc(myDisplay);// Callback function
	glutSpecialFunc(processArrowKeys); // Arrow Key controls
	glutKeyboardFunc(processKeyboard); // Letter Key controls
	glutPassiveMotionFunc(processMouse);
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
	initScene();
	glutMainLoop();// Display everything and wait
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
	(*v)[0] = ((*v)[0] * screenWidth) / (fov * (*v)[3]);// + screenWidth/2;
	(*v)[1] = ((*v)[1] * screenHeight) / (fov * (*v)[3]); //+ screenHeight/2;	
}

void drawPerspectiveSimple(void(*drawLineFunc)(float x1, float y1, float x2, float y2))
{
	int trignum = trig.trigNum();
	Vector3f v1,v2,v3;
	Vector4f pv1,pv2,pv3; // Homogenous vectors for homogenous transformations
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
	glColor3f(lineR, lineG, lineB);
	/*Above as in drawOrthogonalSimple(*)*/
	glBegin(GL_POINTS);	
	for (int i = 0 ; i < trignum; i++)  
	{
		trig.getTriangleVertices(i,v1,v2,v3);
		pv1=v1,pv2=v2,pv3=v3;		
		//Transform object

		matDotVec(translationMatrix, &pv1);
		matDotVec(rotationMatrix, &pv1);
		matDotVec(scaleMatrix, &pv1);
		matDotVec(shearMatrix, &pv1);

		matDotVec(translationMatrix, &pv2);
		matDotVec(rotationMatrix, &pv2);
		matDotVec(scaleMatrix, &pv2);
		matDotVec(shearMatrix, &pv2);

		matDotVec(translationMatrix, &pv3);
		matDotVec(rotationMatrix, &pv3);
		matDotVec(scaleMatrix, &pv3);
		matDotVec(shearMatrix, &pv3);

		//Perform perspective projection for each pixel and draw
		projectPerspective(&pv1);
		projectPerspective(&pv2);
		projectPerspective(&pv3);		
			//Draw lines
			drawLineFunc(pv1[0],pv1[1],pv2[0],pv2[1]);
			drawLineFunc(pv2[0],pv2[1],pv3[0],pv3[1]);
			drawLineFunc(pv3[0],pv3[1],pv1[0],pv1[1]);	
	}
	glEnd();
}

void drawOrthogonalSimple(void (*drawLineFunc)(float x1, float y1, float x2, float y2))
{
	int trignum = trig.trigNum();
	Vector3f v1,v2,v3;

	glColor3f(1,1,1);  // change the colour of the pixel


	//
	// for all the triangles, get the location of the vertices,
	// project them on the xy plane, and color the corresponding pixel by white
	//

	for (int i = 0 ; i < trignum; i++)  
	{
		/*** do the rasterization of the triangles here using glRecti ***/
		trig.getTriangleVertices(i, v1,v2,v3);

		//
		// colouring the pixels at the vertex location 
		// (just doing parallel projectiion to the xy plane. 
		// only use glBegin(GL_POINTS) for rendering the scene  
		//
		glBegin(GL_POINTS);	
			//Draw lines
			drawLineFunc(v1[0],v1[1],v2[0],v2[1]);
			drawLineFunc(v2[0],v2[1],v3[0],v3[1]);
			drawLineFunc(v3[0],v3[1],v1[0],v1[1]);

		glEnd();	
	}
}

