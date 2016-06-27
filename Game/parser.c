#include "g_array.h"
#include "stdio.h"
#include "stdlib.h"

void GetNextNumber(char** line, char* number) 
{
	int count = 0;

	memset(number, 0, 10);
	
	while( *(*line) != '/' && *(*line) != ' ' && *(*line) != '\n' && *(*line) != '\0' ) 
	{ 
		number[count] = *(*line); 
		++count; 
		++(*line); 
	}
}

void GetLineFromFile(char** buffer, char* line)
{
	int count = 0;
	char key = *(*buffer);

	memset(line, 0, 56);

	while( key != '\n' && key != '\0')
	{
		line[count] = key;
		++(*buffer);
		++count;
		key = *(*buffer);
	}

	if(key != '\0')
		++(*buffer);
}

float GetCoordFromLine(char** line)
{
	char number[10];
	memset(number, 1, 10);

	++(*line);

	GetNextNumber(line, number);

	return (float)atof(number);
}

void GetFacesFromLine(char** line, Array* indices, Array* uvs, Array* normals)
{
	int index = 0;
	char number[10];
	memset(number, 0, 10);

	++(*line);

	GetNextNumber(line, number);
	index = atoi(number);
	ARRAY_PUSH_BACK(indices, int, index - 1);

	++(*line);

	GetNextNumber(line, number);
	index = atoi(number);
	ARRAY_PUSH_BACK(uvs, int, index - 1);

	++(*line);

	GetNextNumber(line, number);
	index = atoi(number);
	ARRAY_PUSH_BACK(normals, int, index - 1);
}

int main(int argc, char** argv)
{
	char* filename = argv[1];

	Array vertices = {0};
	Array indices = {0};
	Array uvs = {0};
	Array uv_indices = {0};
	Array normal_indices = {0};
	Array normals = {0};

	float* uvs_corrected;
	float* normals_corrected;

	char* buffer;
	char* buffer_free;
	char* pos;
	char line[56];
	char key;

	int size = 0;
	int bytes = 0;
	float coord = 0;
	int face = 0;
	int index = 0;
	int uv_index = 0;
    int vert_index = 0;

	FILE* file;

	file = fopen(filename, "r");

	if(!file)	return 0;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	buffer = (char*)malloc(size + 10);
	memset(buffer, 0, size + 10);

	bytes = fread(buffer, sizeof(char), size, file);

	buffer_free = buffer;

	fclose(file);

	//if(bytes != size)	return;
	pos = buffer;

	while( *buffer != '\0' )
	{
		GetLineFromFile(&buffer, line);
		pos = line;
		key = *pos;

		if( key == 'v' )
		{
			++pos;
			key = *pos;

			if( key == 't' )
			{
				++pos;
				while( *pos != '\n' && *pos != '\0' )
				{
					coord = GetCoordFromLine(&pos);
					ARRAY_PUSH_BACK(&uvs, float, coord);
				}
			}
			if( key == 'n' )
			{
				++pos;
				while( *pos != '\n' && *pos != '\0' )
				{
					coord = GetCoordFromLine(&pos);
					ARRAY_PUSH_BACK(&normals, float, coord);
				}
			}
			else
			{
				while( *pos != '\n' && *pos != '\0' )
				{
					coord = GetCoordFromLine(&pos);
					ARRAY_PUSH_BACK(&vertices, float, coord);
				}
			}
		}
		else if( key == 'f' )
		{ 
			++pos;
			while( *pos != '\n' && *pos != '\0' )
			{
				GetFacesFromLine(&pos, &indices, &uv_indices, &normal_indices);
			}
		}

	}

	uvs_corrected = (float*)malloc(uvs.size * sizeof(float));
    uv_index = 0;
    vert_index = 0;
    for(index = 0; index < uv_indices.size; ++index) 
	{
        uv_index = ARRAY_GET(&uv_indices, int, index);
        vert_index = ARRAY_GET(&indices, int, index);
        uvs_corrected[vert_index * 2] = ARRAY_GET(&uvs, float, (uv_index * 2));
        uvs_corrected[(vert_index * 2) + 1] = ARRAY_GET(&uvs, float, (uv_index * 2) + 1);
    }

	size = (indices.size + vertices.size + uvs.size + normals.size) * 4;

	file = fopen(argv[2], "wb");

	fwrite((const void*)size, sizeof(int), 1, file);
	fwrite((const void*)vertices.size, sizeof(int), 1, file);
	fwrite((const void*)vertices.data, sizeof(float), vertices.size, file);
	
	fwrite((const void*)indices.size, sizeof(int), 1, file);
	fwrite((const void*)indices.data, sizeof(int), indices.size, file);
	
	fwrite((const void*)uvs.size, sizeof(int), 1, file);
	fwrite((const void*)uvs.data, sizeof(float), uvs.size, file);
	
	fwrite((const void*)normals.size, sizeof(int), 1, file);
	fwrite((const void*)normals.data, sizeof(float), normals.size, file);

	fclose(file);

	return 0;
}

