#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#include "GL\glew.h"
#include "g_common.h"
#include "g_game.h"
#include "g_loader.h"

#define print(A) char buffer[256];wsprintf(buffer, "FPS: %d\n", A);OutputDebugString(buffer);

int Running;
int GlobalPerfCountFrequency;
HCURSOR cursor;

LARGE_INTEGER Win32GetWallClock(void)
{    
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return(Result);
}

float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	float Result;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

    Result = ((float)(End.QuadPart - Start.QuadPart) / (float)Frequency.QuadPart);
    return(Result);
}

void SetupPixelFormat(HDC hDC)
{
        /*      Pixel format index
        */
        int nPixelFormat;

        static PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
                1,                                      //default version
                PFD_DRAW_TO_WINDOW |                    //window drawing support
                PFD_SUPPORT_OPENGL |                    //opengl support
                PFD_DOUBLEBUFFER,                       //double buffering support
                PFD_TYPE_RGBA,                          //RGBA color mode
                32,                                     //32 bit color mode
                0, 0, 0, 0, 0, 0,                       //ignore color bits
                0,                                      //no alpha buffer
                0,                                      //ignore shift bit
                0,                                      //no accumulation buffer
                0, 0, 0, 0,                             //ignore accumulation bits
                16,                                     //16 bit z-buffer size
                0,                                      //no stencil buffer
                0,                                      //no aux buffer
                PFD_MAIN_PLANE,                         //main drawing plane
                0,                                      //reserved
                0, 0, 0 };                              //layer masks ignored

                /*      Choose best matching format*/
                nPixelFormat = ChoosePixelFormat(hDC, &pfd);

                /*      Set the pixel format to the device context*/
                SetPixelFormat(hDC, nPixelFormat, &pfd);
}

//User Defined Function that Handles Messages sent to the Window
LRESULT CALLBACK MyWindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	LRESULT Result = 0;


	switch(Message)
    {
    	case WM_CREATE:
    	{
    		
    	} break;
    	case WM_DESTROY:
    	{
			Running = 0;
	        PostQuitMessage(WM_QUIT);
	        break;
	    }
        default:
        {
			Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

	return Result;
}

void ProcessMessages(GameState* state)
{
	MSG Message;
	LARGE_INTEGER new_clock;
	Entity* player = state->player;

	while( PeekMessage(&Message, 0, 0, 0, PM_REMOVE) )
	{	
		switch(Message.message)
		{
			case WM_KEYDOWN:
		    {
		    	switch (Message.wParam) 
	            { 
	            	case 0x57:
	            		GetForwardVector(player->direction, player->model);
	                    break;  
	                case 0x41:
                		GetRightVector(player->direction, player->model);
                		VectorNormalize(player->direction);
                        GetVectorInverse(player->direction);
	                    break; 
	                case 0x53:
	            		GetForwardVector(player->direction, player->model);
	            		VectorNormalize(player->direction);
                        GetVectorInverse(player->direction);
	                    break;  
	                case 0x44:
	            		GetForwardVector(player->direction, player->model);
                		GetRightVector(player->direction, player->model);
	                    break;
	                case 0x45: //E
	            		state->keys = state->keys | 1;
	                	break; 
	                case 0x51: //Q
	            		state->keys = state->keys | 2;
	                    break;
					case 0x52: //R
						state->keys = state->keys | 4;
						break;
	                case VK_LEFT:
	                    break; 
	                case VK_RIGHT: 
	                    break; 
	                case VK_UP: 
	                    break; 
	                case VK_DOWN: 
	                    break;                 
	                default: 
	                    break; 
	            } 
		    } break;
		    case WM_KEYUP:
		    {
				new_clock = Win32GetWallClock();
				
				if( state->key == Message.wParam )
				{
					state->time = Win32GetSecondsElapsed(state->clock, new_clock);
				}
				else
				{
					state->key = Message.wParam;
				}

		    	switch (Message.wParam) 
	            { 
	            	case 0x57: //W
	            		state->keys = state->keys ^ 1;
	                    break;  
	                case 0x41: //A
	            		state->keys = state->keys ^ 2;
	                    break; 
	                case 0x53: //S
	            		state->keys = state->keys ^ 4;
	                    break;  
	                case 0x44: //D
	            		state->keys = state->keys ^ 8;
	                    break;
	                case 0x45: //E
	            		state->keys = state->keys ^ 16;
	                	break; 
	                case 0x51: //Q
	            		state->keys = state->keys ^ 32;
	                    break;
					case 0x52: //R
						state->keys = state->keys ^ 64;
						break;
	                case VK_LEFT:
	                    break; 
	                case VK_RIGHT: 
	                    break; 
	                case VK_UP: 
	                    break; 
	                case VK_DOWN: 
	                    break;           
	                default: 
	                    break; 
	            } 
		    } break;
			default:
			{

			} break;
		}

		state->clock = Win32GetWallClock();

		TranslateMessage(&Message);
	    DispatchMessageA(&Message);
	}
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowFlags)
{
	DWORD error;
	int SleepIsGranular = 0;
	UINT DesiredSchedulerMS = 1;

	float MSPerFrame = 0;
	float TargetSecondsPerFrame;
    float WorkSecondsElapsed;
	float SecondsElapsedForFrame;
	float TestSecondsElapsedForFrame;

	int width;
	int height;

	LARGE_INTEGER LastCounter;
	LARGE_INTEGER FlipWallClock;
	LARGE_INTEGER WorkCounter;
	LARGE_INTEGER EndCounter;

	uint64 LastCycleCount;
	HWND Window;
	RECT ClientRect;
	DWORD SleepMS;

	HGLRC RC;

	GLenum err = 0;

	GameState state = {0};
	Camera camera = {0};
	Entity player = {0};
	Entity ground = {0};
	Renderer renderer;
	Texture cube_t = {0};
	Texture terrain_t = {0};

	void* Mesh = 0;

	WNDCLASSEX WindowClass;
	WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = MyWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = 0;
    WindowClass.hCursor = 0;
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = "WindowClass";
    WindowClass.hIconSm = 0;

	if( RegisterClassEx(&WindowClass) )
	{
		width = GetSystemMetrics(SM_CXMAXTRACK);
		height = GetSystemMetrics(SM_CYMAXTRACK);

		Window = CreateWindowExA(0, WindowClass.lpszClassName, "My Test Window",
									  WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_MAXIMIZE,
									  0, 0, width, height,
									  0, 0, Instance, 0);

		TargetSecondsPerFrame = 1.0f / 60.0f;

		if(Window)
		{
			HDC DC = GetDC(Window);
			SetupPixelFormat(DC);
            RC = wglCreateContext(DC);
            wglMakeCurrent(DC, RC);

			err = glewInit();
			if (GLEW_OK != err)
				fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

			GetClientRect(Window, &ClientRect);

			width = ClientRect.right - ClientRect.left;
			height = ClientRect.bottom - ClientRect.top;

			glViewport(0,0, width, height);
			glEnable(GL_DEPTH_TEST);

			InitGame(&state);
			GetMeshAssets(&state);
			InitCamera(&camera, width, height);
			InitRenderer(&renderer);

			CreateTexture(&cube_t, "../assets/textures/cube.png");
			CreateTexture(&terrain_t, "../assets/textures/Sand2.jpg");

			InitEntity(&player, GetMeshReference(&state, "player"), &cube_t);
			player.type = PLAYER;
			LoadEntityIntoRenderer(&renderer, &player);

			InitEntity(&ground, GetMeshReference(&state, "arena"), &terrain_t);
			player.type = TERRAIN;
			LoadEntityIntoRenderer(&renderer, &ground);

			state.deltaT = TargetSecondsPerFrame;
			state.camera = &camera;
			state.player = &player;
			state.renderer = &renderer;

			LastCounter = Win32GetWallClock();
			state.clock = LastCounter;
			FlipWallClock = Win32GetWallClock();
			
    		SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

			LastCycleCount = __rdtsc();

			Running = 1;

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			while(Running)
			{

				ProcessMessages(&state);

				if(!state.keys)
				{
					state.time = Win32GetSecondsElapsed( state.clock, Win32GetWallClock() );
				}

				//Clear the screen to black
		        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				UpdateAndRender(&state);

				WorkCounter = Win32GetWallClock();
                WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
				SecondsElapsedForFrame = WorkSecondsElapsed;

				if(SecondsElapsedForFrame < TargetSecondsPerFrame)
				{                        
				    if(SleepIsGranular)
				    {
				        SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
				        if(SleepMS > 0)
				        {
				            Sleep(SleepMS);
				        }
				    }

				    TestSecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
				    if(TestSecondsElapsedForFrame < TargetSecondsPerFrame)
				    {
				        //LOG MISSED SLEEP HERE
				    }

				    while(SecondsElapsedForFrame < TargetSecondsPerFrame)
				    {                            
				        SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
				    }
				}
				else
				{
				    //MISSED FRAME!
				    //Logging
				}

				EndCounter = Win32GetWallClock();
				MSPerFrame = 1000.0f * Win32GetSecondsElapsed(LastCounter, EndCounter);                    
				LastCounter = EndCounter;

				FlipWallClock = Win32GetWallClock();

				SwapBuffers(DC);
			}
		}
		else
		{
			error = GetLastError();
		}
	}
	else
	{
		error = GetLastError();
	}

	return 0;
}