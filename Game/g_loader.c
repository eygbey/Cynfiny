#include "g_array.h"
#include "g_mesh.h"

typedef struct
{
	V3* v;
	V2* uv;
	V3* n;

	void* buffer;
} Loader;

void ParseObj(Loader* loader, char* filename)
{
	if(filename)
	{
		FileContents contents = GetFileContents(filename);


	}
}