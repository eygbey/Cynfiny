#ifndef G_RENDERER_H
#define G_RENDERER_H

const char FREE = 1 << 0;
const char INDEXED = 1 << 1;
const char ORTHOGRAPHIC = 1 << 2;
const char PERSPECTIVE = 1 << 3;
const char TEXTURED = 1 << 4;

const u32 MAX_RENDERABLES = 2048;

// typedef enum
// {
// 	MODEL, MODEL_TEX, UI
// } RenderType;


typedef struct
{
	V3 c;
	V3 r;

	u32 tbo;

	u32 vao;
	u32 program;
	u32 num_indices;
}EntryModel;


typedef struct
{
	V3 color;
	V2 offset;
	u32 vao;

	// u32 tbo;
	// u32 program;
}EntryUI;

typedef struct
{
	EntryUI entries[256];

	u32 num_entries;

	u32 font_program;
	u32 ui_program;
}UIRenderer;

typedef struct
{
	EntryModel entries[1024];

	u32 num_entries;

	u32 tex_program;
	u32 flat_program;
}ModelRenderer;


typedef struct  
{
	M4 orthographic;
	M4 perspective;
	M4 view;

	UIRenderer ui;
	ModelRenderer model;
} Renderer;

#endif