#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "demo4.h"

int nRows = 480;
int nCols = 480; 
Volume vol;

static int spin = 0;
static int spin2 = 0;
static int begin;
static int begin2;

int button_id; 
int state_id;

Vector3f L(0,0,1);
Vector3f V(0,0,1);
float tmpf = 0.2;

//Keys
const int KEY_1 = 49;
const int KEY_2 = 50;
const int KEY_3 = 51;
const int KEY_4 = 52;
const int KEY_5 = 53;
const int KEY_6 = 54;
const int KEY_PLUS = 61;
const int KEY_MINUS = 45;

//Modes
int const MODE_GREYSCALE_DENSITY = 0;
int const MODE_GREYSCALE_DEPTH = 1;
int const MODE_GREYSCALE_MAXVALUE = 2;
int const MODE_GREYSCALE_AVERAGEVALUE = 3;
int const MODE_GREYSCALE_COMPOSITE_FRONT = 4;
int const MODE_GREYSCALE_COMPOSITE_BACK = 5;
int mode = MODE_GREYSCALE_COMPOSITE_BACK;

//View modes
int const VIEW_FRONT = 0;
int const VIEW_BACK = 1;
int const VIEW_LEFT = 2;
int const VIEW_RIGHT = 3;
int const VIEW_TOP = 4;
int const VIEW_BOTTOM = 5;
int view_mode = VIEW_FRONT;

//Alpha and colour transfer textures
float alphaArray[256];
float colourArray[256][3];

vector<TransferControlPoint> colourKnots = vector<TransferControlPoint>();
vector<TransferControlPoint> alphaKnots = vector<TransferControlPoint>();

void controlPoints1()
{
	alphaKnots.clear();
	colourKnots.clear();
	//Control points 1
	// Assumes that the knots are sorted on the int argument
	alphaKnots.push_back(TransferControlPoint(0.0f, 0));
	alphaKnots.push_back(TransferControlPoint(0.0f, 0.65)); //Start skin
	alphaKnots.push_back(TransferControlPoint(0.02f, 68));
	alphaKnots.push_back(TransferControlPoint(0.02f, 102));
	alphaKnots.push_back(TransferControlPoint(0.0f, 105)); // End skin
	alphaKnots.push_back(TransferControlPoint(0.f, 127)); //Start bone
	alphaKnots.push_back(TransferControlPoint(0.9f, 128));
	alphaKnots.push_back(TransferControlPoint(0.9f, 240)); //End bone
	alphaKnots.push_back(TransferControlPoint(0.f, 256)); 

	// Assumes that the knots are sorted on the int argument
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 0));
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 65)); //Start skin
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 102)); //End skin
	colourKnots.push_back(TransferControlPoint(1.0f, 1.0f, .95f, 128)); //Start bone
	colourKnots.push_back(TransferControlPoint(1.0f, 1.0f, .95f, 256)); //End bone
}

void controlPoints2()
{
	alphaKnots.clear();
	colourKnots.clear();
	//Control points 2
	// Assumes that the knots are sorted on the int argument
	alphaKnots.push_back(TransferControlPoint(0.0f, 0));
	alphaKnots.push_back(TransferControlPoint(0.0f, 0.65)); //Start skin
	alphaKnots.push_back(TransferControlPoint(0.005f, 68));
	alphaKnots.push_back(TransferControlPoint(0.005f, 102));
	alphaKnots.push_back(TransferControlPoint(0.0f, 105)); // End skin
	alphaKnots.push_back(TransferControlPoint(0.f, 124)); //Start bone
	alphaKnots.push_back(TransferControlPoint(0.05f, 128));
	alphaKnots.push_back(TransferControlPoint(0.05f, 252)); 
	alphaKnots.push_back(TransferControlPoint(0.f, 254)); //End bone
	alphaKnots.push_back(TransferControlPoint(0.f, 256)); //End bone

	// Assumes that the knots are sorted on the int argument
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 0));
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 65)); //Start skin
	colourKnots.push_back(TransferControlPoint(.91f, .7f, .61f, 102)); //End skin
	colourKnots.push_back(TransferControlPoint(1.0f, 1.0f, .95f, 128)); //Start bone
	colourKnots.push_back(TransferControlPoint(1.0f, 1.0f, .95f, 256)); //End bone
}

void createTransferFunctions()
{
	for (int i = 0; i < 256; i++)
	{
		int a = 0; // lower knot index
		int b = 0; // upper knot index
		
		//alphaArray[i] = 0.1;

		//Populate alpha array
		for (int j = 0; j < alphaKnots.size(); j++)// Find a pair of knots
		{
			if (i < alphaKnots[j].scalar) // find the upper knot -> the first with the scalar higher than i
			{
				b = j;
				break;
			}
		}
		a = b - 1; // the lower knot is always the one below

		float c = (float)(alphaKnots[b].scalar - i)/(float)(alphaKnots[b].scalar - alphaKnots[a].scalar);
		alphaArray[i] = alphaKnots[a].alpha*(c) + alphaKnots[b].alpha*(1-c); // linearly interpolate

		colourArray[i][0] = 0.7;
		//Populate colour array
		for (int j = 0; j < colourKnots.size(); j++)// Find a pair of knots
		{
			if (i < colourKnots[j].scalar) // find the upper knot -> the first with the scalar higher than i
			{
				b = j;
				break;
			}
		}
		a = b - 1; // the lower knot is always the one below

		c = (float)(colourKnots[b].scalar - i)/(float)(colourKnots[b].scalar - colourKnots[a].scalar);
		colourArray[i][0] = colourKnots[a].colour[0]*(c) + colourKnots[b].colour[0]*(1-c); // linearly interpolate
		colourArray[i][1] = colourKnots[a].colour[1]*(c) + colourKnots[b].colour[1]*(1-c); // linearly interpolate
		colourArray[i][2] = colourKnots[a].colour[2]*(c) + colourKnots[b].colour[2]*(1-c); // linearly interpolate

	}
	/*for (int j = 0; j < 256; j++)// Find a pair of knots
	{
		cout << "Alpha array: \n";
		cout << j << " : " << alphaArray[j] << "\n";
	}*/
	int hist[256];for (int j = 0; j < 256; j++) hist[j] = 0;
	for (int xi = 1 ; xi < vol.sizex()-1 ; xi++)  
	{
		for (int yi = 1 ; yi < vol.sizey()-1 ; yi++)  
		{
			for (int zi = 1 ; zi < vol.sizez()-1 ; zi++) 
			{
				hist[vol.volume(xi, yi, zi)]++;
			}
		}
	}
	cout << "Histogram: \n";
	for (int j = 0; j < 256; j++)// Find a pair of knots
	{
		cout << j << " : " << hist[j] << "\n";
	}
}


void Volume::load(char * filename)
{  
	int   temp;
	int   data_compression;
	int   plane_size;
	int   volume_size;
	double f[3];
	int   size[3];
	int   magic_num;
	int   y_counter;
	int   icon_width, icon_height;
	int   compressed_size;

	unsigned char *compressed_ptr;
	unsigned char *scan_ptr = NULL;

	ifstream fin;
	ofstream fout("test");

	fin.open(filename);

	// Initialize
	if (fin == NULL)
	{
		cerr << filename << " cannot be found " << endl;
		return;
	}

	fin >> size[2] >> size[0] >> size[1];


	plane_size = size[2] * size[0];
	volume_size = plane_size * size[1];

	_volume = new unsigned short * [size[1]]; 
	for (int i = 0; i < size[1];i++) 
	{
		_volume[i] = new unsigned short [plane_size]; 
	}

	// Read In Data Plane By Plane
	for( y_counter=0; y_counter<size[1]; y_counter++ ) 
	{
		for (int i = 0; i < size[2]; i++) 
		{
			for (int j = 0; j < size[0]; j++) {
				_volume[size[1]-1 - y_counter][i*size[2] + j]=fin.get();
				fin >>_volume[size[1]-1 - y_counter][i*size[2] + j];
	//			fout.put(_volume[size[1]-1 - y_counter][i*size[2] + j]);
			}
		}
	}

	_size[0] = size[0];
	_size[1] = size[1];
	_size[2] = size[2];

	cerr <<"Read " << volume_size << " points" << endl;
}

void drawPixel(float intensityX, float intensityY, float intensityZ, int xi, int yi)
{
	glColor3f(intensityX,intensityY,intensityZ);
	glRectf(xi-vol.sizex()/2 , 2*yi -vol.sizey(),
	xi-vol.sizex()/2+1, 2*yi-vol.sizey()+2);
}

// The opacity transfer function
void getOpacity(int scalar, float *opacity)
{
	*opacity = alphaArray[scalar];
	/**opacity = (1-scalar/255.f)/100;*/
}

// The colour transfer function
void getColour(int scalar, float *colour)
{
	colour[0] = colourArray[scalar][0];
	colour[1] = colourArray[scalar][1];
	colour[2] = colourArray[scalar][2];
	/*static float C, V, S, H, _H, X;
	V = 0.7; S = 0.7;
	C = V * S;
	H = 360.f*scalar/200;
	_H = H/60.f;
	X = C*(1- abs(fmod(_H,2) -1));
	if (0 <= _H && _H < 1) { colour[0] = C; colour[1] = X; colour[2] = 0; return;}
	if (_H < 2) { colour[0] = X; colour[1] = C; colour[2] = 0; return;}
	if (_H < 3) { colour[0] = 0; colour[1] = C; colour[2] = X; return;}
	if (_H < 4) { colour[0] = 0; colour[1] = X; colour[2] = C; return;}
	if (_H < 5) { colour[0] = X; colour[1] = 0; colour[2] = C; return;}
	if (_H < 6) { colour[0] = C; colour[1] = 0; colour[2] = X; return;}*/
}

void transform1(int xi, int yi, int zi, int *xt, int *yt, int *zt) //done
{
	*xt = xi;
	*yt = yi;
	*zt = zi;
}

void transform2(int xi, int yi, int zi, int *xt, int *yt, int *zt)
{
	static int sizez = vol.sizez();
	*xt = xi;
	*yt = yi;
	*zt = sizez-zi;
}

void transform3(int xi, int yi, int zi, int *xt, int *yt, int *zt)
{
	static int sizex = vol.sizex();
	static int sizez = vol.sizez();
	*xt = sizez-zi;
	*yt = yi;
	*zt = sizex-xi;
}

void transform4(int xi, int yi, int zi, int *xt, int *yt, int *zt) //done
{
	static int sizex = vol.sizex();
	static int sizez = vol.sizez();
	*xt = zi;
	*yt = yi;
	*zt = xi;
}
void transform5(int xi, int yi, int zi, int *xt, int *yt, int *zt)
{
	static int sizez = vol.sizez();
	*xt = xi;
	*yt = sizez-zi;
	*zt = yi;
}

void transform6(int xi, int yi, int zi, int *xt, int *yt, int *zt)
{
	static int sizey = vol.sizey();
	*xt = xi;
	*yt = zi;
	*zt = sizey-yi;
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear OpenGL Window

	Vector3f v1,v2,v3;

	Vector3f N;
	int threshold = tmpf * 255.f;  // if you change this value and you will see different contours
	float cumulativeOpacity, opacity, tmpc, intensityX, intensityY, intensityZ = 0;
	float colour[3];
	float cumulativeIntensities[3];
	for (int xi = 1 ; xi < vol.sizex()-1 ; xi++)  
	{
		for (int yi = 1 ; yi < vol.sizey()*3 ; yi++)  
		{
			//cout << "(x,y)="<< xi << " " << yi << ":"; 
			tmpc = 0;
			cumulativeOpacity = 0;
			intensityX, intensityY, intensityZ = 0;
			cumulativeIntensities[0] = cumulativeIntensities[1] = cumulativeIntensities[2] = 0;
			if (mode != MODE_GREYSCALE_COMPOSITE_BACK)
			{
				for (int zi = 1 ; zi < vol.sizez()-1 ; zi++) 
				{
					// Transform xi, yi, zi
					static int xt,yt,zt;
					switch (view_mode)
					{
						case VIEW_FRONT:
							transform1(xi,yi,zi,&xt,&yt,&zt);
							break;
						case VIEW_BACK:
							transform2(xi,yi,zi,&xt,&yt,&zt);
							break;
						case VIEW_LEFT:
							transform3(xi,yi,zi,&xt,&yt,&zt);
							break;
						case VIEW_RIGHT:
							transform4(xi,yi,zi,&xt,&yt,&zt);
							break;
						case VIEW_TOP:
							transform5(xi,yi,zi,&xt,&yt,&zt);
							break;
						case VIEW_BOTTOM:
							transform6(xi,yi,zi,&xt,&yt,&zt);
							break;
					}

					if (vol.volume(xt,yt,zt) > threshold) 
					{
						switch (mode)
						{
							case MODE_GREYSCALE_DENSITY:
								tmpc = (float)vol.volume(xt,yt,zt)/255.f; // Colour and opacity functions go here
								intensityX = intensityY = intensityZ = tmpc;
								drawPixel(intensityX, intensityY, intensityZ, xt, yt);
								goto exit_loop;
								break;
							case MODE_GREYSCALE_DEPTH:
								tmpc = (float)(vol.sizez() - zt -1)/(vol.sizez()-1);
								intensityX = intensityY = intensityZ = tmpc;
								drawPixel(intensityX, intensityY, intensityZ, xt, yt);
								goto exit_loop;
								break;
							case MODE_GREYSCALE_MAXVALUE:
								if (tmpc < (float)vol.volume(xt,yt,zt)) tmpc = (float)vol.volume(xt,yt,zt);
								break;
							case MODE_GREYSCALE_AVERAGEVALUE:
								tmpc += (float)vol.volume(xt,yt,zt);
								break;
							case MODE_GREYSCALE_COMPOSITE_FRONT:
								getOpacity(vol.volume(xt,yt,zt), &opacity); // Get opacity for this pixel
								getColour(vol.volume(xt,yt,zt), &colour[0]); // Get pixel colour

								cumulativeOpacity = opacity + (1-opacity)*cumulativeOpacity;

								cumulativeIntensities[0] = cumulativeIntensities[0] + (1 - cumulativeOpacity)*opacity*colour[0];
								cumulativeIntensities[1] = cumulativeIntensities[1] + (1 - cumulativeOpacity)*opacity*colour[1];
								cumulativeIntensities[2] = cumulativeIntensities[2] + (1 - cumulativeOpacity)*opacity*colour[2];

								if (cumulativeOpacity > 0.9f) goto draw_accumulated;
								break;
						}
					}
				}
			}
			draw_accumulated: ;
			switch (mode)
			{
				case MODE_GREYSCALE_MAXVALUE:
					tmpc = (float)tmpc/255.f;
					intensityX = intensityY = intensityZ = tmpc;
					drawPixel(intensityX, intensityY, intensityZ, xi, yi);
					break;
				case MODE_GREYSCALE_AVERAGEVALUE:
					tmpc = tmpc/vol.sizez()/255.f;
					intensityX = intensityY = intensityZ = tmpc;
					drawPixel(intensityX, intensityY, intensityZ, xi, yi);
					break;
				case MODE_GREYSCALE_COMPOSITE_FRONT:
					intensityX = cumulativeIntensities[0];
					intensityY = cumulativeIntensities[1];
					intensityZ = cumulativeIntensities[2];
					drawPixel(intensityX, intensityY, intensityZ, xi, yi);
					break;
				case MODE_GREYSCALE_COMPOSITE_BACK:
					tmpc = tmpc/255.f;
					intensityX = intensityY = intensityZ = tmpc;
					drawPixel(intensityX, intensityY, intensityZ, xi, yi);
					break;
			}
			exit_loop: ;
		}
	}

	glFlush();// Output everything
}

void processKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case KEY_1:
			mode = MODE_GREYSCALE_DENSITY;
			break;
		case KEY_2:
			mode = MODE_GREYSCALE_DEPTH;
			break;
		case KEY_3:
			mode = MODE_GREYSCALE_MAXVALUE;
			break;
		case KEY_4:
			mode = MODE_GREYSCALE_AVERAGEVALUE;
			break;
		case KEY_5:
			controlPoints1(); createTransferFunctions();
			mode = MODE_GREYSCALE_COMPOSITE_FRONT;
			break;
		case KEY_6:
			controlPoints2(); createTransferFunctions();
			mode = MODE_GREYSCALE_COMPOSITE_FRONT;
			break;
		case KEY_PLUS:
			view_mode = (view_mode+1)%6;
			cout << "view_mode " << view_mode << "\n";
			break;
		case KEY_MINUS:
			view_mode = view_mode-1;
			view_mode = view_mode < 0 ? view_mode + 6 : view_mode;
			cout << "view_mode" << view_mode << "\n";
			break;
		default:
			mode = MODE_GREYSCALE_DENSITY;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	if (argc >  1)  {
		vol.load(argv[1]);
	}
	else {
		cerr << argv[0] << " <filename> " << endl;
		exit(1);
	}

	int width, height;
	controlPoints1(); createTransferFunctions();
	mode = MODE_GREYSCALE_COMPOSITE_FRONT;
	glutInit(&argc, argv);
	glutInitWindowSize(nRows, nCols);
	glutCreateWindow("Volume Example");
	glutKeyboardFunc(processKeyboard);
	gluOrtho2D(-nRows/2, nRows/2, -(float)nCols/2,  (float)nCols/2);
	glutDisplayFunc(myDisplay);// Callback function
	glutMainLoop();// Display everything and wait
};
