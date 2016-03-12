#ifndef LINEDRAWING_H
#define LINEDRAWING_H

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
	float lineR = 0.5, lineG = 0.5, lineB = 0.5;	
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

int sign(float n)
{
	if (n > 0) return 1;
	return -1;
}

#endif
