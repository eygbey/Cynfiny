u32 LoadElement(V2 dim)
{
	GLuint vao, bo, ibo;

	float x = dim.x, y = dim.y;

	V2 data[8] = {
		#if 1
		{0.0f, 0.0f},
		{x, 0.0f},
		{0.0f, y},
		{x, y},
		#else
		{-x, y},
		{x, y},
		{-x, -y},
		{x, -y},
		#endif
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f}
	};

	u32 i[6] = {0, 1, 2, 3, 2, 1};

	{
    	u32 error = glGetError();

    	ASSERT(error == 0);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers( 1, &bo );
    glBindBuffer(GL_ARRAY_BUFFER, bo);
    glBufferData(GL_ARRAY_BUFFER, 64, data, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,  2, GL_FLOAT, GL_FALSE, 0, 0);//vertex data
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,  2, GL_FLOAT, GL_FALSE, 0, data + 4);//uv data
    glEnableVertexAttribArray(1);

    glGenBuffers( 1, &ibo );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24, i, GL_STATIC_DRAW);

    {
    	u32 error = glGetError();

    	ASSERT(error == 0);
    }

    ASSERT(vao > 0);

    return vao;
}

void ActivateParent(UIManager* manager, u32 index, u16 offset_to_children, u8 num_children)
{
	UIActiveElement* active = manager->active_elements;
	UIElement* element = manager->elements + index;

	UIActiveElement* parent_of_element = active + element->parent_index;
	
	UIActiveElement to_be_active = {};

	V2 dim = element->dim;
	V2 offset = element->offset_from_parent;
	V2 parent_corner = parent_of_element->corner;

	V2 rel_corner = {parent_corner.x + offset.x, parent_corner.y + offset.y};
	V2 center = {rel_corner.x + dim.x/2.0f, rel_corner.y + dim.y/2.0f};
	
	UIParent parent = {dim, center, num_children, offset_to_children};

	to_be_active.vao = LoadElement(dim);
	to_be_active.corner = rel_corner;
	to_be_active.color = element->color;

	manager->active_elements[manager->active_count++] = to_be_active;
	manager->parents[manager->parent_count++] = parent;
}

void ActivateChild(UIManager* manager, u32 index)
{
	UIActiveElement* active = manager->active_elements;
	UIElement* element = manager->elements + index;

	UIActiveElement* parent = active + element->parent_index;
	
	UIActiveElement to_be_active = {};

	V2 dim = element->dim;
	V2 offset = element->offset_from_parent;
	V2 parent_corner = parent->corner;

	V2 rel_corner = {parent_corner.x + offset.x, parent_corner.y + offset.y};
	V2 center = {rel_corner.x + dim.x/2.0f, rel_corner.y + dim.y/2.0f};
	
	UIChild child = {dim, center};

	to_be_active.vao = LoadElement(dim);
	to_be_active.corner = rel_corner;
	to_be_active.color = element->color;

	manager->active_elements[manager->active_count++] = to_be_active;
	manager->children[manager->child_count++] = child;
}

void Deactivate(UIManager* manager)
{

}

//NOTE: Need to Autogenerate this function for easier iteration
void OnClickEvent(UIManager* manager, u32 id)
{
	switch(id)
    {
        case 0:
        {

        } break;
        default:
        {} break;
    }
}

void LoadTestUI(UIManager* manager, float width, float height)
{
	{
		UIElement bar;
		bar.dim = {width, 32.0f};
		bar.offset_from_parent = {0.0f, 0.0f};
		bar.parent_index = 0;
		bar.color = {1.0f, 0.4f, 0.7f};
		manager->elements[manager->deactive_count++] = bar;
		ActivateParent(manager, 0, 0, 2);
	
		UIElement bar2;
		bar2.dim = {32.0f, 32.0f};
		bar2.offset_from_parent = {0.0f, 0.0f};
		bar2.parent_index = 1;
		bar2.color = {1.0f, 1.0f, 1.0f};
		manager->elements[manager->deactive_count++] = bar2;
		ActivateChild(manager, 1);
	
		UIElement bar3;
		bar3.dim = {32.0f, 32.0f};
		bar3.offset_from_parent = {34.0f, 0.0f};
		bar3.parent_index = 1;
		bar3.color = {1.0f, 1.0f, 1.0f};
		manager->elements[manager->deactive_count++] = bar3;
		ActivateChild(manager, 2);
	}

	{
		UIElement bar;
		bar.dim = {width, 32.0f};
		bar.offset_from_parent = {0.0f, height - 32.0f};
		bar.parent_index = 0;
		bar.color = {1.0f, 0.4f, 0.7f};
		manager->elements[manager->deactive_count++] = bar;
		ActivateParent(manager, 3, 2, 2);
	
		UIElement bar2;
		bar2.dim = {32.0f, 32.0f};
		bar2.offset_from_parent = {0.0f, 0.0f};
		bar2.parent_index = 3;
		bar2.color = {1.0f, 1.0f, 1.0f};
		manager->elements[manager->deactive_count++] = bar2;
		ActivateChild(manager, 4);
	
		UIElement bar3;
		bar3.dim = {32.0f, 32.0f};
		bar3.offset_from_parent = {34.0f, 0.0f};
		bar3.parent_index = 3;
		bar3.color = {1.0f, 1.0f, 1.0f};
		manager->elements[manager->deactive_count++] = bar3;
		ActivateChild(manager, 5);
	}

}

void SendToRenderer(UIManager* manager, UIRenderer* renderer)
{
	u32 index = 0;
	u32 count = manager->active_count;

	UIActiveElement* active = manager->active_elements;

	EntryUI* entries = renderer->entries;

	while(index < count)
	{
		EntryUI entry = {};
		entry.offset = active->corner;
		entry.vao = active->vao;
		entry.color = active->color;

		entries[renderer->num_entries++] = entry;

		++index;
		++active;
	}
}

u32 CheckRect(UIRect r, V2 coords)
{
	V2 c = r.c;
	V2 d = r.d;

	float min_x = c.x - d.x;
	float min_y = c.y - d.y;
	float max_x = c.x + d.x;
	float max_y = c.y + d.y;

	if(coords.x >= min_x &&
	   coords.x <= max_x &&
	   coords.y >= min_y &&
	   coords.y <= max_y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

UIParent* SelectUIParent(UIParent* parents, u32 num, V2 coords)
{
	u32 index = 0;

	UIParent* p = 0;

	while(index < num)
	{
		p = parents + index;

		if(CheckRect(p->rect, coords))
		{
			return p;
		}

		++index;
	}

	return 0;
}

int SelectUIChild(UIChild* children, u32 num, V2 coords)
{
	u32 index = 0;

	UIChild* c = 0;

	while(index < num)
	{
		c = children + index;

		if(CheckRect(c->rect, coords))
		{
			return index;
		}

		++index;
	}

	return -1;
}