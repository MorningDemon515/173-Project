
/*
#include "Src\MainScence.h"
#include "Src\Launcher.h"

Menu* menu;
Scence* sce;

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	//menu->Launcher();
	sce->MainWorld();
	return 0;
}
*/

#include "Src/Renderer/Image_GL.h"

#include <GL/glew.h>
#include <SDL.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <windows.h>

Image_GL* imgl;

int main(int argc, char* argv[]) {
    SDL_Event e;//SDL�¼�
    SDL_GLContext GLContext;//OpenGL������

    // ��ʼ�� SDL
    SDL_Init(SDL_INIT_VIDEO);

    // ���� SDL ����
    SDL_Window* window = SDL_CreateWindow(" ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);//��������

    GLContext = SDL_GL_CreateContext(window);//����OpenGL������
    SDL_GL_MakeCurrent(window, GLContext);//����OpenGL������

    glewExperimental = GL_TRUE;//��ʼ��GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //������ѭ��
    bool running = true;
    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            case SDL_MOUSEMOTION:

                break;

            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                {

                }
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        imgl->Draw_BackGround_GL("GFX/menu/back.jpg");

        SDL_GL_SwapWindow(window);

        SDL_Delay(1);

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}