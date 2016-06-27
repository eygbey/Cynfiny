#ifndef G_CAMERA_H
#define G_CAMERA_H

#include "g_math.h"

#define R0TATION_SPEED 0.01f
#define MOVEMENT_SPEED 2.0f
#define PITCH 0
#define YAW 1

typedef struct 
{
    M4 perspective;
    M4 view;

	V3 position;
	V3 rotation;

} Camera;

#include "g_camera.c"

#endif