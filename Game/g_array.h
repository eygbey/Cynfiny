#ifndef G_ARRAY_H
#define G_ARRAY_H

#include "g_common.h"
#include <sys/types.h>
#include <string.h>

typedef struct 
{
	void* data;
	int size;
	int max_size;
} Array;

#define INIT_ARRAY(da) \
	(da)->data = malloc(500); \
	(da)->max_size = 500; \
	(da)->size = 0;

#define REALLOC(mem, bytes) realloc(mem, bytes);

#define ARRAY_PUSH_BACK(da, type, ele) \
    if( (da)->size == (da)->max_size ) \
    { \
        (da)->max_size = ( (da)->max_size + 1 ) * 2; \
        (da)->data = REALLOC( (da)->data, (da)->max_size * sizeof(type) ) \
    } \
    ((type*)(da)->data)[(da)->size] = ele; \
    (da)->size += 1;

#define ARRAY_GET(da, type, i) ((type*)(da)->data)[i]

#define TRANSFER_ARRAY_DATA(da, a, type) \
    (a) = REALLOC( (a), sizeof(type) * (da)->size); \
	memcpy( (a), (da), sizeof(type) * (da)->size );
    
#define FREE_ARRAY(a) free( (a)->data );

#endif