#ifndef G_TEXTURE_H
#define G_TEXTURE_H

typedef struct 
{
	u32 type;	
	int width;           	/* Image width in pixels */
	int height;
	char* pixels;
} Texture;

typedef struct
{
	u32 type;
	u32 offset;
	short width;
	short height;
} TextureHeader;

typedef struct
{
	u32 type;
	short width;
	short height;
} TextureFileHeader;

typedef struct
{
	TextureHeader* headers;

	char* pixels;

	int size;
	int capacity;

	int total_offset;
} TextureManager;


#endif