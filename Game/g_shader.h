#ifndef G_SHADER_H
#define G_SHADER_H

const u32 MAX_PROGRAMS = 25;

typedef struct 
{
	GLuint id;
} Shader;

typedef struct 
{
	u32 num;

	char* offsets;
	
	unsigned short* programs;
	unsigned short* ids;
} ShaderManager;

#include "g_shader.c"

#endif