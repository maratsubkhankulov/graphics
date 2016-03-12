Matric number: s0806287

To compile:
	open terminal, cd into directory, type "make"
	or
	g++ -o teapot teapot.cc imageloader.h -lglut -lGLU -lGL

To run:

	./teapot MIT_teapot.obj

Basic Criteria:

1.Computed Vertex Normals - see screenshot "vertexNormals.png"
2.Flat shading - see screenshot "flat_clean.png"
3.Phong shading - see screenshot "phong_clean.png"
4.Table - also visible in the above screenshots
5.Z-buffer to hidden surface removal

Advanced Criteria:

1.Texture mapping - simply done by loading the image "rock.bmp" and using it's 
	indices for diffuse color values. Used the imageloading (.h,.cpp) code from the CG website.
	See "texture.png" for details.

Controls: (Built on previous submission, but emphasis wasn't put on interaction)

	Control / Function:

	Mouse / to look
	Arrow keys / scale,rotate

Rendering options:
	
	Edit "renderingoptions.h" for various modes of rendering & recompile (possibly need recompile manually):
	
	bBarycentric - rasterize using barycentric coordinates
	bWireframe - draw wireframe
	bNormals - render vertex normals

	BASELINE - flat colour
	FLAT - flat shading
	PHONG_1 - directional light/Light vector never changes during computation
	PHONG_2 - light vector computed from light position to interpolated point of the triangle
	TEXTURE - my texture mapping attempt

Discussion:

	Shading althought easy to understand on paper becomes quickly confusing when implementing. 
	As a result profiling tools and auto tests become very useful, as the details and quickly become overwhelming
	and counter-intuitive. It's easy to get into the routine of playing around with variables in hope of a pretty picture;
	Hours can go by without you noticing.
