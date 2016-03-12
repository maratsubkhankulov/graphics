#ifndef RENDERING_OPTIONS_H
#define RENDERING_OPTIONS_H

bool bWireframe = false;
bool bBarycentric = true;
bool bScanline = false;
bool bNormals = false;

/* Shading modes */
const int BASELINE = 0;
const int FLAT = 1;
const int PHONG_1 = 2;
const int PHONG_2 = 3;
const int TEXTURE = 4;
int SHADING_MODE = TEXTURE;

#endif
