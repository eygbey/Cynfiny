#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

#include "GL\glew.h"

#include "g_math.h"

int Running;
int GlobalPerfCountFrequency;

typedef struct{
  	void* contents;
  	int size;
}FileContents;

#define PLATFORM_READ(name) FileContents name(char *fname)
typedef PLATFORM_READ(PlatformReadFile);

PLATFORM_READ(GetFileContents){
	FileContents result = {};

  	HANDLE fileHandle = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    
    if(fileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER fileSize;
        if(GetFileSizeEx(fileHandle, &fileSize))
        {
            uint32_t fileSize32 = (uint32_t)fileSize.QuadPart;
            result.contents = VirtualAlloc(0, fileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            if(result.contents)
            {
                DWORD bytesRead;
                if(ReadFile(fileHandle, result.contents, fileSize32, &bytesRead, 0) &&
                   (fileSize32 == bytesRead))
                {
                    result.size = fileSize32;
                }
                else
                {             
                    VirtualFree(result.contents, 0, MEM_RELEASE);
                    result.contents = 0;
                }
            }
            else
            {
            }
        }
        else
        {
        }

        CloseHandle(fileHandle);
    }
    else
    {
    }

    return result;

}

#define PLATFORM_WRITE(name) int name(char *fname, FileContents* file)
typedef PLATFORM_WRITE(PlatformWriteFile);

PLATFORM_WRITE(WriteFileContents){

  	HANDLE fileHandle = CreateFileA(fname, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    
    DWORD bytesWritten = 0;

    if(fileHandle != INVALID_HANDLE_VALUE)
    {
        uint32_t fileSize32 = file->size;
        
        if(file->contents)
        {

            if(WriteFile(fileHandle, file->contents, fileSize32, &bytesWritten, 0) &&
               (fileSize32 == bytesWritten))
            {
                file->size = fileSize32;
            }else{
            	bytesWritten = 0;
            }
        }     

        CloseHandle(fileHandle);
    }

    return bytesWritten;
}



LARGE_INTEGER Win32GetTime(void)
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

void* GetMemory(int size){
   return VirtualAlloc(0, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

}

#include "g_math.h"
#include "g_game.h"
#include "g_game.c"

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
		case WM_SIZE:
		{

			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			{
				u32 width = ClientRect.right - ClientRect.left;
				u32 height = ClientRect.bottom - ClientRect.top;

				glViewport(0, 0, width, height);
			}
		} break;
        default:
        {
			Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

	return Result;
}

void ProcessMessages(GameState* state)
{
	uint32_t* key = &state->keys;
	uint32_t* lock = &state->locks;
	MSG Message;

	while( PeekMessage(&Message, 0, 0, 0, PM_REMOVE) )
	{	
		switch(Message.message)
		{
			case WM_MOUSEMOVE:
			{
				POINT p;
				GetCursorPos(&p);
				state->mouse.current.x = (float)p.x;
				state->mouse.current.y = (float)p.y;
			} break;
			case WM_LBUTTONDOWN:
			{
				state->mouse.left_click = true;
			} break;
			case WM_LBUTTONUP:
			{
				state->mouse.left_click = false;
			} break;
			case WM_RBUTTONDOWN:
			{
				state->mouse.right_click = true;
			} break;
			case WM_RBUTTONUP:
			{
				state->mouse.right_click = false;
			} break;
			case WM_KEYDOWN:
		    {
		    	switch(Message.wParam){
		    		case 0x57: //W
				    {	    	
				    	if( !(*lock & W_KEY) ){
				    		*key |= W_KEY;
				    	}
				    } break;
				    case 0x41:
				    {		    	
				    	if( !(*lock & A_KEY) ){
				    		*key |= A_KEY;
				    	}
				    } break;
				    case 0x53://S
				    {	    	
				    	if( !(*lock & S_KEY) ){
				    		*key |= S_KEY;
				    	}
				    } break;
				    case 0x44:
				    {		    	
				    	if( !(*lock & D_KEY) ){
				    		*key |= D_KEY;
				    	}
				    } break;
				    case 0x5A:
				    {
				    	if(*key & Z_KEY){
				    		*key ^= Z_KEY;
				    	}else{
				    		*key |= Z_KEY;
				    	}	
				    } break;
				    case VK_LEFT:
				    {		    	
				    	if( !(*lock & LEFT_KEY) ){
				    		*key |= LEFT_KEY;
				    	}
				    } break;
				    case VK_RIGHT:
				    {		    	
				    	if( !(*lock & RIGHT_KEY) ){
				    		*key |= RIGHT_KEY;
				    	}
				    } break;
				    case VK_UP:
				    {	    	
				    	if( !(*lock & UP_KEY) ){
				    		*key |= UP_KEY;
				    	}
				    } break;
				    case VK_DOWN:
				    {		    	
				    	if( !(*lock & DOWN_KEY) ){
				    		*key |= DOWN_KEY;
				    	}
				    } break;
				    case 0x31:
				    {
				    	*key |= ONE_KEY;
				    } break;
				    case 0x32:
				    {
				    	*key |= TWO_KEY;           
				    } break;
				    case 0x33:
				    {
				    	*key |= THREE_KEY;           
				    } break;
				    case VK_SHIFT:
				    {
				    	*key |= SHIFT_KEY;
				    } break;
				    case VK_SPACE:
				    {   
				    	if(!(*lock & SPACE_KEY))
				    	{
				    		*key |= SPACE_KEY;
				    	}
				    } break;
				    case VK_ESCAPE:
				    {
						if (!(*lock & ESC_KEY)){
							*key |= ESC_KEY;
						}
				    } break;
				    case VK_CONTROL:
				    {
						*key |= CTRL_KEY;
				    } break;
				    case VK_MENU:
				    {
						*key |= ALT_KEY;
				    } break;
				    case VK_RETURN:
				    {   
				    	*key |= ENTER_KEY;
				    } break;
				    case VK_F1:
				    {   
				    	*key |= F1_KEY;
				    } break;
				    case VK_F2:
				    {   
				    	*key |= F2_KEY;
				    } break;
				    default:
				    	break;

		    	}
		    } break;
		    case WM_KEYUP:
		    {
				switch(Message.wParam){
		    		case 0x57: //W
				    {
				    	*key &= ~W_KEY;
				    	if(*lock & W_KEY){
				    		*lock ^= W_KEY;
				    	}
				    } break;
				    case 0x41:
				    {
				    	*key &= ~A_KEY;
				    	if(*lock & A_KEY){
				    		*lock ^= A_KEY;
				    	}
				    } break;
				    case 0x53://S
				    {
				    	*key &= ~S_KEY;
				    	if(*lock & S_KEY){
				    		*lock ^= S_KEY;
				    	}
				    } break;
				    case 0x44:
				    {
				    	*key &= ~D_KEY;
				    	if(*lock & D_KEY){
				    		*lock ^= D_KEY;
				    	}
				    } break;
				    case VK_LEFT:
				    {
				    	*key &= ~LEFT_KEY;
				    	if(*lock & LEFT_KEY){
				    		*lock ^= LEFT_KEY;
				    	}
				    } break;
				    case VK_RIGHT:
				    {
				    	*key &= ~RIGHT_KEY;
				    	if(*lock & RIGHT_KEY){
				    		*lock ^= RIGHT_KEY;
				    	}
				    } break;
				    case VK_UP:
				    {
				    	*key &= ~UP_KEY;
				    	if(*lock & UP_KEY){
				    		*lock ^= UP_KEY;
				    	}
				    } break;
				    case VK_DOWN:
				    {
				    	*key &= ~DOWN_KEY;
				    	if(*lock & DOWN_KEY){
				    		*lock ^= DOWN_KEY;
				    	}
				    } break;
				    case VK_SPACE:
				    {
				    	
				    } break;
				    case 0x31:
				    {
				    	*key ^= ONE_KEY;
				    } break;
				    case 0x32:
				    {
				    	*key ^= TWO_KEY;           
				    } break;
				    case 0x33:
				    {
				    	*key ^= THREE_KEY;           
				    } break;
				    case VK_SHIFT:
				    {
				    	*key ^= SHIFT_KEY;
				    } break;
				    case VK_CONTROL:
				    {
						*key ^= CTRL_KEY;
				    } break;
					case VK_ESCAPE:
					{
						*key ^= ESC_KEY;
					} break;
				    case VK_RETURN:
				    {   
						*key &= ~ESC_KEY;
						if (*lock & ESC_KEY) {
							*lock ^= ESC_KEY;
						}
				    } break;
				    default:
				    	break;

		    	}
		    } break;
			default:
			{

			} break;
		}

		TranslateMessage(&Message);
	    DispatchMessageA(&Message);
	}
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowFlags)
{
	DWORD error;
	UINT DesiredSchedulerMS = 1;

	float MSPerFrame = 0;
	float TargetSecondsPerFrame;

	int width;
	int height;
	
	HWND Window;
	RECT ClientRect;

	HGLRC RC;

	GLenum err = 0;

	GameState state = {0};

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
		width = GetSystemMetrics(SM_CXMAXIMIZED);
		height = GetSystemMetrics(SM_CYMAXIMIZED);

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

			state.width = (float)width;
			state.height = (float)height;

			glViewport(0,0, width, height);
			glEnable(GL_DEPTH_TEST);

			//V3 p1 = { -8.0f, 6.0f, 4.3f };
			V3 p1 = { 0.0f, 6.0f, 4.0f };
			InitCamera(&state.main, p1);
			
			state.deltaT = TargetSecondsPerFrame;
			
			InitGame(&state);
			
			Running = 1;

			LARGE_INTEGER start, end;

			float timeElasped;
			DWORD sleepMS;

			while(Running)
			{
				start = Win32GetTime();

				ProcessMessages(&state);

				if(state.keys & Z_KEY){
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		    	}else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				
				UpdateAndRender(&state);

				end = Win32GetTime();
				timeElasped = Win32GetSecondsElapsed(start, end);

				if(timeElasped < TargetSecondsPerFrame){
					sleepMS = (DWORD)((TargetSecondsPerFrame*1000) - (timeElasped*1000));
					Sleep(sleepMS);
				}else{
					OutputDebugString("Missed Frame");
				}


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