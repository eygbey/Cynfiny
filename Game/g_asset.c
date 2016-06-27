void AllocateBufferManager(BufferManager* manager, int size)
{
    char* memory = (char*)GetMemory( (size * sizeof(Buffer)) + (size * sizeof(u32) * 2) + size );
    manager->buffers = (Buffer*)memory;
    manager->vaos = (u32*)(memory + size * sizeof(Buffer));
    manager->tbos = manager->vaos + size;
    manager->flags = memory + (sizeof(Buffer) * size) + (sizeof(u32) * 2 * size);
}

Buffer GetBuffer(BufferManager* manager, int index)
{
    return manager->buffers[index];
}

u32 GetVAO(BufferManager* manager, int index)
{
    return manager->vaos[index];
}

void PushBuffer(BufferManager* manager, Buffer buffer, int index)
{
    manager->buffers[index] = buffer;
    ++manager->num_buffers;
}

void FreeBuffer(BufferManager* manager, int i)
{
    Buffer buffer = GetBuffer(manager, i);
    u32 vao = GetVAO(manager, i);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(4, buffer.id);

    buffer = {};

    manager->buffers[i] = buffer;
    --manager->num_buffers;
}

void PushBufferFlag(BufferManager* manager, int index, const char flag)
{
    manager->flags[index] |= flag;
}

void PushVAO(BufferManager* manager, u32 vao, int index)
{
    manager->vaos[index] = vao;
}

void PushTBO(BufferManager* manager, u32 tbo, int index)
{
    manager->tbos[index] = tbo;
}