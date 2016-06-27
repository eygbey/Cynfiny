#ifndef G_MESH_H
#define G_MESH_H

#define POSITION_ATTR 0
#define TEXTURE_ATTR 1

const char VERTICES = 1 << 0;
const char UVS = 1 << 1;
const char NORMALS = 1 << 2;
const char INDICES = 1 << 3;
const char TWO_D = 1 << 4;

#if 0
typedef struct
{
	float* vertices;
	float* uvs;
	float* normals;
	int* indices;

	int num_attrs;
	int num_indices;
} Mesh;
#else
typedef struct
{
	float* data;
	u32 data_size;

	int* indices;
	u32 num_indices;
} Mesh;
#endif

typedef struct
{
	uint16_t num_indices;
	uint16_t num_attrs;//number of vertices,normals,uvs
} MeshFileHeader;

typedef struct
{
	u32 offset;

	uint16_t num_indices;
	uint16_t num_attrs;

	char flags;
} MeshHeader;

typedef struct
{
	u32 size;
	u32 capacity;

	u32 total_offset;//in bytes
	
	//Double Buffer
	MeshHeader* headers;
	Value* values;
} StaticMeshManager;

#endif