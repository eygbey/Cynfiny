#ifndef G_MATH_H
#define G_MATH_H

#include <math.h>

#define PI 3.14159f
#define M4SIZE sizeof(float) * 16
#define V3SIZE sizeof(float) * 3
#define V4SIZE sizeof(float) * 4

#define DegToRad(deg) (deg * PI / 180)

#define X 0
#define Y 1
#define Z 2
#define W 3

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct{
	float data[16];
} M4;

typedef struct{
	float x;
	float y;
	float z;
	float w;
} V4;

typedef struct{
	float x;
	float y;
	float z;
} V3;

typedef struct{
	float x;
	float y;
} V2;

#include "g_math.c"

#endif