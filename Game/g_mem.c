typedef struct
{
	int size;
	int max;

	Value* values;
} Stack;

typedef struct
{
	int size;
	int max;

	Value* values;
} DoubleBuffer;

typedef struct
{
	int size;
	int max;

	Value* front;
	Value* back;
} DoubleEndedStack;

typedef struct
{
	int size;
	int max;

	int current;

	Value* values;
} Queue;

typedef struct
{
	int size;
	int max;

	int front;
	int back;

	Value* values;
} DoubleEndedQueue;