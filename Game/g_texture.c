#include "g_texture.h"

#define CreateTexture(id) glGenTextures(1, &id);

void AllocateTextureManager(TextureManager* manager, int size)
{
	manager->total_offset = 0;
	manager->size = 0;
	manager->capacity = size;

    char *buffer = (char*)GetMemory(size);
	manager->pixels = buffer + size - 1;
	manager->headers = (TextureHeader*)(buffer);
}

void PushTextureFromFile(TextureManager* manager, char* filename, int index)
{
	char* pixels;

	int size = 0, new_size = 0;

	TextureFileHeader* file_header;
	TextureHeader* header;
	
	FileContents file = GetFileContents(filename);

	size = file.size - sizeof(TextureFileHeader);
	new_size = manager->size + size + sizeof(TextureHeader);

	ASSERT(manager->capacity > new_size);

	file_header = (TextureFileHeader*)file.contents;

	header = manager->headers + index;

	header->offset = manager->total_offset + size;
	header->width = file_header->width;
	header->height = file_header->height;
	header->type = file_header->type;

	pixels = (char*)(file.contents);

	memcpy(manager->pixels - header->offset, pixels + sizeof(TextureFileHeader), size);

	manager->total_offset = header->offset;

	manager->size = new_size;

	VirtualFree(file.contents, 0, MEM_RELEASE);
}

Texture GetTexture(TextureManager* manager, int index)
{
	Texture result = {};

	TextureHeader* header = manager->headers + index;

	result.type = header->type;
	result.width = header->width;
	result.height = header->height;
	result.pixels = manager->pixels - header->offset;

	return result;
}

#define UseTexture(id)	glBindTexture(GL_TEXTURE_2D, id);

void LoadTexture(unsigned char* pixels, int width, int height, GLuint type)
{
  	if( pixels )
	{
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
	}
}

void SetActiveTexture(Texture* t)
{

}