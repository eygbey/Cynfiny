typedef enum UIType {};

typedef struct
{
	V2 d;//In Pixels
	V2 c;//In Screen Coords
} UIRect;

typedef struct
{
	u32 vao;
	u32 parent_index;

	V2 corner;
	V3 color;

} UIActiveElement;

typedef struct
{
	UIRect rect; 
} UIChild;

typedef struct
{
	UIRect rect;

	u16 offset_to_children;

	u8 num_children;
} UIParent;

typedef struct
{
	V3 color;
	V2 dim;
	V2 offset_from_parent;

	u32 parent_index;
}UIElement;


const u16 PARENT = 0xFFFF;

typedef struct
{
	//hash table
	u32 ids[256];
	UIElement elements[256];

	UIActiveElement active_elements[100];//active elements only
	UIParent parents[100];
	UIChild children[256];

	V2 window_metrics;

	u32 active_count;
	u8 deactive_count;

	u8 child_count;
	u8 parent_count;

} UIManager;