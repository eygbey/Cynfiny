#include "g_math.h"

typedef struct
{
	uint16_t offset;
	uint16_t size;
} Header;

typedef struct
{
	union
	{
		Header h;
		int i;
		float f;
	};
} Value;

#include "g_camera.h"
#include "g_physics.h"
#include "g_renderer.h"
#include "g_entity.h"
#include "g_mesh.h"
#include "g_asset.h"
#include "g_texture.h"
#include "g_font.h"
#include "g_shader.h"
#include "g_ui.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define Kilobytes(b)  b * 1024
#define Megabytes(b)  b * (1024 * 1024)
#define Gigabytes(b)  b * (1024 * 1024 * 1024)

#define ASSERT(condition)	if(!(condition)){int* check_assert = 0; *check_assert = 0;}

typedef struct
{
	V2 prev;
	V2 current;

	bool left_click;
	bool right_click;

	float speed;
} Mouse;

typedef struct
{
	Rect r;
	Capsule c;

	RigidBody body;

	V3 deltaP;
	V3 deltaW;
	V3 drag;
	V3 speed;

	V3 rotation;

	V3 max_speed;

	float height;
	float oscilation;
	float remaining;

	u32 state;
} Player;

struct GameState 
{
	float width;	//Window Metrics
	float height;	

	Renderer renderer;
	PhysicsManager physics;
	CollisionManager colliders;
	EntityManager entities;
	StaticMeshManager meshes;
	BufferManager buffers;
	ShaderManager shaders;
	TextureManager textures;
	UIManager uis;

	Player player;

	Camera main;
	Mouse mouse;
	
	u32 keys;
	u32 locks;
	
	float deltaT;

	u32 program;

	char phase;

	GLuint font_texture;
	GLuint font_program;
	//FontMeta font_data[96];
	stbtt_bakedchar cdata[96];
};

/***********************
** Keyboard Constants **
************************/

const u32 W_KEY = 1<<0;
const u32 A_KEY = 1<<1;
const u32 S_KEY = 1<<2;
const u32 D_KEY = 1<<3;

const u32 LEFT_KEY = 1<<4;
const u32 RIGHT_KEY = 1<<5;
const u32 UP_KEY = 1<<6;
const u32 DOWN_KEY = 1<<7;

const u32 ONE_KEY = 1<<8;
const u32 TWO_KEY = 1<<9;
const u32 THREE_KEY = 1<<10;
const u32 SPACE_KEY = 1<<11;
const u32 SHIFT_KEY = 1<<12;

const u32 Z_KEY = 1<<13;
const u32 ESC_KEY = 1<<14;
const u32 ENTER_KEY = 1<<15;
const u32 F1_KEY = 1<<16;
const u32 F2_KEY = 1<<17;
const u32 CTRL_KEY = 1<<18;
const u32 ALT_KEY = 1<<19;

#define GAME 	0
#define MAIN 	1
#define PAUSE   2
#define DEBUG   3

#include "g_mesh.c"
#include "g_asset.c"
#include "g_texture.c"
#include "g_renderer.c"
#include "g_entity.c"
#include "g_font.c"
#include "g_physics.c"
#include "g_ui.c"
