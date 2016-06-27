#include <stdio.h>
#include <stdlib.h>
#include <time.h>

V3 ProcessKeyboardMovement(M4 view, u32 key, u32 lock)
{
    V3 deltaP = {};

    if( key & W_KEY ) //W
    {
        GetForwardVector(&deltaP, &view);
        VectorNormalize(&deltaP);
        deltaP.z *= -1;
    } 
    if( key & A_KEY )
    {
        GetRightVector(&deltaP, &view);
        VectorNormalize(&deltaP);
        deltaP.x *= -1;
        GetVectorInverse(&deltaP);
    } 
    if( key & S_KEY )//S
    {
        GetForwardVector(&deltaP, &view);
        VectorNormalize(&deltaP);
        deltaP.z *= -1;
    } 
    if( key & D_KEY )
    {
        GetRightVector(&deltaP, &view);
        VectorNormalize(&deltaP);
        deltaP.x *= -1;
        GetVectorInverse(&deltaP);
    }

    return deltaP;
}

V3 ProcessMouseMovement(Mouse mouse)
{
    V3 deltaP = {};

    return deltaP;
}

int ProcessKeyobardInput(GameState* state)
{
    u32* keyP = &state->keys;
    u32* lock = &state->locks;
    u32 key = *keyP;

	Renderer* renderer = &state->renderer;
    Player* player = &state->player;

    SetDefaultVector(&player->deltaP);

    //TODO: Interpolate This Value!
    if( key & LEFT_KEY )
    {
        player->rotation.y -= 0.05f;
    }
    if( key & RIGHT_KEY )
    {
        player->rotation.y += 0.05f;
    }
    if( key & UP_KEY )
    {
        player->rotation.x -= 0.05f;
    }
    if( key & DOWN_KEY )
    {
        player->rotation.x += 0.05f;
    }

    if( key & SPACE_KEY )
    {
        
    }

    if( key & ONE_KEY )
    {

    }

    if( key & CTRL_KEY )
    {
        

    }
    else
    {
        
    }

    if( key & ALT_KEY )
    {
        

    }

    if( key & TWO_KEY )
    {
        //state->main = state->c2;            
    }

    if( key & THREE_KEY )
    {
        //state->main = state->c3;            
    }
    if( key & F1_KEY )
    {
		if(key & F2_KEY)
			*keyP ^= F2_KEY;
    }
    if( key & F2_KEY )
    {
		if (key & F1_KEY)
			*keyP ^= F1_KEY;
    }
    if( key & SHIFT_KEY )
    {            
    }else{
    }
    if( key & ESC_KEY )
    {

    }

    return 0;
}

void UpdateAndRender(GameState* state)
{
    Renderer* renderer = &state->renderer;
    
	//Clear the screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(state->phase == MAIN)
    {
        
    }

    if(state->phase == GAME)
    {
        PhysicsManager* physics = &state->physics;
        CollisionManager* colliders = &state->colliders;
        UIManager* uis = &state->uis;
        Mouse* mouse = &state->mouse;

        if(mouse->left_click)
        {
            UIParent* parent = SelectUIParent(uis->parents, uis->parent_count, mouse->current);
            if(parent)
            {
                UIChild* children = uis->children + parent->offset_to_children;

                u32 index = SelectUIChild(children, parent->num_children, mouse->current);
                if(index != -1)
                {
                    u32 id = parent->offset_to_children + index;

                    
                }
            }
        }

        V3 deltaP = ProcessKeyboardMovement(renderer->view, state->keys, state->locks);

#if 0
        {
            float x = 0;
            float y = 32;

			char string[256] = {};
            sprintf(string, "%f %f %f", player->deltaP.x, player->deltaP.y, player->deltaP.z);

            PrintText(state, "Delta: ", &x, &y);
            PrintText(state, string, &x, &y);

            x = 0;
            y += 32;

            PrintText(state, "State: ", &x, &y);

            if(player->state & JUMPING)
                PrintText(state, "Jumping", &x, &y);
            else if(player->state & STANDING)
                PrintText(state, "Standing", &x, &y);
            else if(player->state & CROUCHING)
                PrintText(state, "Crouching", &x, &y);
            else if(player->state & FALLING)
                PrintText(state, "Falling", &x, &y);
            else if(player->state & LANDING)
                PrintText(state, "Landing", &x, &y);
        }
#endif
        //Start(renderer);
        {
            UIRenderer* ui = &renderer->ui;

            ui->num_entries = 0;

            SendToRenderer(uis, ui);

            Render(renderer);
        }
        
    }
}



#define ZeroMem(block, size)\
{\
    for(int i = 0; i < size; ++i){\
        *(block++) = 0;\
    }\
}

// void InitPlayer(Player* player)
// {
//     Rect r = {{0.0f, 0.0f, 0.0f},{0.5f, 0.5f, 0.5f}};

//     SetVectorFromValues(&player->drag, -15.1f, -9.8f, -15.1f);
//     SetVector(&player->speed, SPEED);
//     SetDefaultVector(&player->rotation);
//     player->height = STANDING_HEIGHT;

//     player->position = {0.0f, 2.0f, 5.0f};
//     player->rotation = MakeZeroV3();
//     player->r = r;
//     player->state |= GROUNDED;
// }

void LoadTestSetup(GameState* state)
{
    PhysicsManager* physics = &state->physics;
    Renderer* renderer = &state->renderer;
    StaticMeshManager* meshes = &state->meshes;
    EntityManager* entities = &state->entities;
    BufferManager* buffers = &state->buffers;
    ShaderManager* shaders = &state->shaders;
    TextureManager* textures = &state->textures;
    CollisionManager* colliders = &state->colliders;


    {
        Shader shader;
        GLuint program;

        Buffer buffer = {};

        // CreateProgram(&program);
        // CreateShader(&shader, "../shaders/basic.vs", GL_VERTEX_SHADER);
        // LoadShader(program, shader.id);
        // CreateShader(&shader, "../shaders/basic.fs", GL_FRAGMENT_SHADER);
        // LoadShader(program, shader.id);
        // LinkProgram(program);

        

        // state->program = program;
     
        // CreateProgram(&state->font_program);
        // CreateShader(&shader, "../shaders/font.vs", GL_VERTEX_SHADER);
        // LoadShader(state->font_program, shader.id);
        // CreateShader(&shader, "../shaders/font.fs", GL_FRAGMENT_SHADER);
        // LoadShader(state->font_program, shader.id);
        // LinkProgram(state->font_program);
		{
			u32 error = glGetError();

			ASSERT(error == 0);
		}

        CreateProgram(&program);
        CreateShader(&shader, "../shaders/ui.vs", GL_VERTEX_SHADER);
        LoadShader(program, shader.id);
        CreateShader(&shader, "../shaders/ui.fs", GL_FRAGMENT_SHADER);
        LoadShader(program, shader.id);
        LinkProgram(program);

        {
            u32 error = glGetError();

            ASSERT(error == 0);
        }

        renderer->ui.ui_program = program;
    }

    {
        unsigned char ttf_buffer[1<<20];
        unsigned char temp_bitmap[512*512];

        fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/times.ttf", "rb"));
        stbtt_BakeFontBitmap(ttf_buffer,0, 25.0, temp_bitmap,512,512, 32,96, state->cdata); // no guarantee this fits!
    
        glGenTextures(1, &state->font_texture);
        glBindTexture(GL_TEXTURE_2D, state->font_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
        // can free temp_bitmap at this point
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    PushRect(colliders, {0.0f, 0.5f, 0.0f}, {0.5f,0.5f,0.5f});
    PushRect(colliders, {0.0f, 0.5f, 0.5f}, {0.5f,0.5f,0.5f});
}

void InitGame(GameState* state)
{
    Camera* camera = &state->main;

    Player* player = &state->player;

    PhysicsManager* physics = &state->physics;
    CollisionManager* colliders = &state->colliders;
    Renderer* renderer = &state->renderer;
    StaticMeshManager* meshes = &state->meshes;
    EntityManager* entities = &state->entities;
    BufferManager* buffers = &state->buffers;
    ShaderManager* shaders = &state->shaders;
    TextureManager* textures = &state->textures;
    UIManager* uis = &state->uis;

    AllocateRenderer(renderer, MAX_RENDERABLES);
    AllocatePhysicsManager(physics, MAX_ENTITIES);
    AllocateCollisionManager(colliders, Megabytes(32));
    AllocateEntityManager(entities, MAX_ENTITIES);
    //AllocateMeshManager(meshes, Megabytes(32));
    AllocateBufferManager(buffers, MAX_ENTITIES);
    AllocateShaderManager(shaders, MAX_ENTITIES);
    AllocateTextureManager(textures, Megabytes(32));

    GetPerspectiveMatrix(&renderer->perspective, 45.0f, (float)(state->width/state->height), 0.1f, 2000.0f);
    GetOrthoMatrix(&renderer->orthographic, 0.0f, state->width, 0.0f, state->height, -0.1f, 100.0f);

    //InitPlayer(player);

    state->phase = GAME;

    UpdateViewMatrix(&renderer->view, camera->position, camera->rotation);

    LoadTestSetup(state);

    LoadTestUI(uis, state->width, state->height);
}

/*
vao = PushAssetFromFile(meshes, "C:\\Users\\Rob\\Desktop\\Cynfiny\\res\\models\\cube.cyn");

GetMM(&r_entry.model, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
r_entry.vao = vao;
r_entry.program = program;
r_entry.numIndices = 36;
r_entry.flags |= PERSPECTIVE;
PushRenderEntry(&r_entry, renderer);

vao = PushAssetFromFile(meshes, "C:\\Users\\Rob\\Desktop\\Cynfiny\\res\\models\\plane.cyn");

GetMM(&r_entry.model, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
r_entry.vao = vao;
r_entry.program = program;
r_entry.numIndices = 36;
r_entry.flags |= PERSPECTIVE;
PushRenderEntry(&r_entry, renderer);
*/