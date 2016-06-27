void AllocateEntityManager(EntityManager* manager, int max)
{
	u32 *buffer = (u32*)GetMemory(max * sizeof(int) + max * sizeof(char) + max * sizeof(Entity) + max * sizeof(u32));
	
	manager->entities = buffer;
	manager->entity_flags = manager->entities + max;
	manager->free_indices = manager->entity_flags + max;

	manager->num_entities = 0;
	manager->num_indices = 0;
	manager->index_counter = 0;
	manager->indices_front = 0;
	manager->indices_back = 0;
}

void SetEntityFlag(EntityManager* manager, u32 flag, int index)
{
	manager->entity_flags[index] |= flag;
}

void ClearEntityFlag(EntityManager* manager, u32 flag, int index)
{
	manager->entity_flags[index] ^= flag;
}

void ClearAllEntityFlags(EntityManager* manager, int index)
{
	manager->entity_flags[index] = 0;
}

u32 GetIndex(EntityManager* manager, int slot)
{
	return manager->entities[slot];
}

u32 CreateEntity(EntityManager* manager)
{
	u32 index;

	if(manager->num_indices)
	{
		if(manager->indices_front == MAX_FREE_INDICES)
			manager->indices_front = 0;

		index = manager->free_indices[manager->indices_front++];
		--manager->num_indices;
		
	}
	else
	{
		index = manager->index_counter++;
	}

	ASSERT(index <= MAX_ENTITIES);

	++manager->num_entities;

	//int slot = manager->num_entities++;

	manager->entities[index] = index;
	manager->entity_flags[index] = ALIVE;

	return index;
}

void DestroyEntity(EntityManager* manager, int slot)
{
	u32 index = manager->entities[slot];
	
	u32* entities = manager->entities;

	if(manager->indices_front == manager->indices_back)
		manager->indices_front = manager->indices_back = 0;
	else if(manager->indices_back == MAX_FREE_INDICES)
		manager->indices_back = 0;

  	manager->free_indices[manager->indices_back++] = index;

  	++manager->num_indices;

  	manager->entity_flags[slot] = 0;
  	--manager->num_entities;

  	//Move Last Element to Open Slot for easier iteration
  	// manager->entities[slot] = manager->entities[--manager->num_entities];
  	// manager->entity_flags[slot] = manager->entity_flags[manager->num_entities];
}