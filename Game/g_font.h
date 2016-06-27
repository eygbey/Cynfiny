

typedef struct{  
	unsigned short x;     
	unsigned short y;
	unsigned short width;     
	unsigned short height;     
	float xoffset;     
	float yoffset;    
	float xadvance; 
} FontMeta;

typedef struct{
	float vertices[8];
	float uvs[8];
	int indices[6];
} FontQuad;

typedef struct
{
	FontQuad quad;
	V2 position;
} AlignedFont;

typedef struct
{
	
} FontManager;

