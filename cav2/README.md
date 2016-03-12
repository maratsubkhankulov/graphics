Computer Animation and Visualization Coursework 2
Marat Subkhankulov s0806287

Basic features:

	-Maximum intensity 
	-Average intensity
	-Forward raycasting composite intensity
	
	-2 sets of opacity + colour transfer functions:
	
	Transfer function 1: Bones fully opaque, black; Skin pink, low-alpha
	Transfer function 2: Bones white, mid-alpha; Skin pink, low-alpha

	This was realized though opacity and colour control points,
	which were assigned to scalar values, the colour and opacity
	between the control points was interpolated linearly.
	The following page provided eseful information on the
	control point implementation:

	http://graphicsrunner.blogspot.co.uk/2009/01/volume-rendering-102-transfer-functions.html

Advanced features:
	
	- Different views: front, back, left, right, top* and bottom*
	Top and bottom only show a part of the head.

The demo:

	The program has the 6 rendering modes:

	Key:	Mode:
	1	Standard scalars
	2	Depth
	3	Maximum intensity
	4	Average intensity
	5	Composite intensity, Transfer Function 1
	6	Composite intensity, Transfer Function 2
	
	And 6 view angles:
	
	- and + keys to cycle through the views
	
	Note:
	-Standard scalar rendering and depth rendering, don't 
	respond to view change.
	-Option 5 shows off the view changes the best.

