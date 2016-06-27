typedef struct
{
	float* mem;

	int used;
	int capacity;
} GeometryCreator;

void InitGeometryCreator(GeometryCreator* creator, int size)
{
	creator->mem = (float*)GetMemory(size);

	creator->used = 0;
	creator->capacity = size;
}