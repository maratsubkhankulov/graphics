Computer Graphics course work 1.
Marat Subkhankulov s0806287

Basic requirements:

	1.I have implemented the simple slope-intercept algorithm without considering lines where slope > 1.
	2.The evidence for the running program is the screenshot named evidence_basic.png

Extensions:

	3.DDA line drawing
	4.Midpoint line drawing
	5.Gupta-Sproull attempt
	6.Perspective projection
	7.Mouse look
	8.Homogenous transformations - scaling, rotation, translation, shearing
	9.Field of view interactive control

Proof of functionality:

	See screenshots in the folder titled according to the function displayed.

Application controls:

	Control/Functionality:

	1/Slope-intercept
	2/DDA
	3/Midpoint
	4/Gupta-Sproull
	5/Transform
	6/Shear
	0/Defaults
	-/Field of view
	+/Field of view
	W/camera forward
	S/camera backward
	D/camera rotate left
	A/camera rotate right
	LEFT/rotate object left
	RIGHT/rotate object right
	UP/scale up along y axis
	DOWN/scale down along y axis

Running the Application:

	compile - g++ -o teapot teapot.cc -lglut -lGLU -lGL
	run - ./teapot MIT_teapot.obj

Links:
	
	http://www.lighthouse3d.com/tutorials/glut-tutorial/
	http://en.wikipedia.org/wiki/3D_projection

Analysis:

	In my implementation Midpoint seems to be the better looking line drawing algorithm. It draws fast and the lines are crisp and consistent.

Future extensions:

	Gupta-Sproull doesn't appear to work as it should.
	Clipping during projection is required - when the camera is too close the program attempts to draw lines of an enourmous length.
	Many camera modes can still be implemented such as first person and follow.
