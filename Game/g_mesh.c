#define CreateVAO(vao)  glGenVertexArrays(1, &vao);
#define StartVAO(vao)  glBindVertexArray(vao);
#define EndVAO(vao)    glBindVertexArray(0);

void AllocateMeshManager(StaticMeshManager* manager, int size)
{

    char* buffer = (char*)GetMemory(size);
    
}


int GetMeshSize(MeshHeader header)
{
    int size = 0;//In Bytes


    return size;
}

//NOTE: Static Mesh Data Will Only need one copy per level/level chunck as entire areas/level can be serialized
//and copied once in a big chunk
u32 LoadToVAO(Mesh mesh)
{
    GLuint error, vao, bo, ibo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers( 1, &bo );
    glBindBuffer(GL_ARRAY_BUFFER, bo);
    glBufferData(GL_ARRAY_BUFFER, mesh.data_size, mesh.data, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,  3, GL_FLOAT, GL_FALSE, 0, 0);//vertex data
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,  2, GL_FLOAT, GL_FALSE, 0, 0);//uv data
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,  3, GL_FLOAT, GL_FALSE, 0, 0);//normal data
    glEnableVertexAttribArray(2);

    glGenBuffers( 1, &ibo );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

    error = glGetError();
    ASSERT(error == 0);

    return vao;
}


//Loads Static Mesh Chunk
void PushStaticMeshes(StaticMeshManager* manager, char* filename)
{
    FileContents file = GetFileContents(filename);

    
    VirtualFree(file.contents, 0, MEM_RELEASE);
}