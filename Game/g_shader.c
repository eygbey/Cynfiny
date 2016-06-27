#include "g_shader.h"

void AllocateShaderManager(ShaderManager* manager, int size)
{
	unsigned short* memory = (unsigned short*)GetMemory(size * sizeof(u32));
	manager->programs = memory;
	manager->ids = memory + size;

	manager->num = 0;
}

void PushProgram(ShaderManager* manager, u32 program, int index)
{
	manager->programs[index] = (unsigned short)program;
	++manager->num;
}

int InitShader(Shader* s, char* buffer, GLenum type)
{
	GLint success;
	GLenum error = 0;

	s->id = glCreateShader(type);

	error = glGetError();

	glShaderSource(s->id, 1, (const GLchar**)&buffer, NULL);

	error = glGetError();

	glCompileShader(s->id);
	
	glGetShaderiv(s->id, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
	    GLchar InfoLog[1024];
	    glGetShaderInfoLog(s->id, sizeof(InfoLog), NULL, InfoLog);
	    OutputDebugString(InfoLog);
	    return -1;
	}
	else
	{
		return 1;
	}
}

int CreateShader(Shader* s, char* fname, GLenum type)
{
	FILE* file = fopen(fname, "r");

	int size;
	int bytes;
	int error;

	char buffer[1024];
	memset(buffer, 0, 1024);

	if(!file){
		error = errno;
		OutputDebugString(strerror(errno));
		return -1;
	}
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	bytes = fread(buffer, 1, size, file);

	InitShader(s, buffer, type);

	return bytes;
}

void CreateProgram(GLuint* program)
{
	*program = glCreateProgram();
}

void LoadShader(GLuint program, GLuint id)
{
    glAttachShader(program, id);   
}

void LinkProgram(GLuint program)
{
	GLint success;
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) 
	{
		GLchar ErrorLog[1024];
	    glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
	    OutputDebugString(ErrorLog);
	    exit(1);
	}
}

void TurnOnShader(GLuint program)
{
	glUseProgram(program);
}

void TurnOffShader()
{
	glUseProgram(0);
}