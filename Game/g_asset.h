const u32 VAO = 0;
const u32 VBO = 0;
const u32 UVBO = 1;
const u32 NBO = 2;
const u32 IBO = 3;

const char FREE_BUFFER = 1 << 0;

typedef struct
{
	u32 id[4];
} Buffer;

typedef struct
{
    //char* offsets;
    
	Buffer* buffers;

	u32* vaos;
	u32* tbos;
	
	char* flags;

    uint16_t num_buffers;
} BufferManager;
