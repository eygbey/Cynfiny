#ifndef G_ENTITY_H
#define G_ENTITY_H

#define INDEX 0x00FFFFFF
#define GENERATION 0xFF000000

#define MAX_ENTITIES 2048

const u32 MAX_FREE_INDICES = 2048;

const u32 DESTROY = 1 << 0;
const u32 LOADED = 1 << 1;
const u32 FONT = 1 << 2;
const u32 UNRENDERABLE = 1 << 3;
const u32 ALIVE = 1 << 4;

typedef struct
{
	uint32_t id;
} Entity;

typedef struct
{
	u32* entities;
	u32* entity_flags;
	u32 num_entities;

	u32* free_indices;
	int num_indices;

	int indices_front;
	int indices_back;

	int index_counter;
} EntityManager;

#endif