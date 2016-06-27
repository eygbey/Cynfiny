typedef struct 
{
	V3 c;
	V3 v;
	V3 w;

	float inverse_mass;

	void* shape;
	u32 size;
	
} RigidBody;

typedef struct
{
	V3 c;
	V3 d;
	//float m;
} Rect;

typedef struct
{ 
	V3 c;
	float r;
} Sphere;

typedef struct
{ 
	V3 a;
	V3 b;
	float r;
} Capsule;

typedef struct
{
	V3 c;
	float points;
} Poly;

typedef struct
{
	u32 actors[2];
	V3 p1;
	V3 p2;
	V3 normal;
	float t;
	float d;
} Contact;

typedef struct
{
	#if 1
	RigidBody* bodies;
	#else
	V3* c;
	V3* r;
	V3* v;
	V3* w;
	#endif
	int count;
} PhysicsManager;

typedef struct
{
	char* shapes;

	int used;
	int capacity;

	int count;
} CollisionManager;

typedef struct
{
	V3 p1;
	V3 p2;

	float rest_dist;

	float k;//Spring Constant
	float b;//Damping Constant;
} Spring;

typedef struct
{
	uint16_t offset;
	uint8_t num_forces;
} ForceHeader;

typedef struct
{
	uint16_t size;
	float* data;
} Force;

typedef struct
{
	float* forces;
	ForceHeader* headers;

	int used;
	int capacity;
} ForcesManager;

const u32 SPHERE_SIZE = sizeof(float);
const u32 RECT_SIZE = sizeof(V3);
const u32 CAPSULE_SIZE = sizeof(Capsule);
const u32 SPRING_SIZE = sizeof(Spring);

/***************************
** Player State Constants **
****************************/

const u32 IDLE = 0;
const u32 SLIDING = 1 << 0;
const u32 WALKING = 1 << 1;
const u32 RUNNING = 1 << 2;
const u32 JUMPING = 1 << 3;
const u32 GROUNDED = 1 << 4;
const u32 CROUCHING = 1 << 5;
const u32 STANDING = 1 << 6;
const u32 CROUCHED = 1 << 7;
const u32 ROLLING = 1 << 8;
const u32 LANDING = 1 << 9;
const u32 INIT_JUMPING = 1 << 10;
const u32 COMPRESSING = 1 << 11;
const u32 EXTENDING = 1 << 12;
const u32 FALLING = 1 << 13;

const V3 AIR_FRICTION = {-0.10f, 0.0f, -0.10f};
const V3 GROUND_FRICTION = {-10.1f, 0.0f, -10.1f};
const V3 SELF_STOPPING = {-20.0f, 1.0f, -20.0f};
const V3 FULL_SPEED = {10.0f, 1.0f, 10.0f};	
const V3 CROUCH_SPEED = {4.0f, 1.0f, 4.0f};	
const V3 SPEED = {5.0f, 1.0f, 5.0f};	

const float CROUCH_OFFSET = 0.2f;
const float STANDING_HEIGHT = 0.5f;
const float CROUCHING_SPEED = -40.1f;
const float OSCILAION_SPEED = 1.0f;
const float MAX_COMPRESSION = 0.1f;

#include "g_rect.c"
#include "g_sphere.c"
#include "g_capsule.c"