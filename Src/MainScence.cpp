#include "MainScence.h"
#include "LoadModel.h"
#include "Launcher.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_Video.h>
#include <iostream>

#include "Image.h"
#include "Text.h"

#include "Text_GL.h"

#include "Shader.h"

SDL_Event mainevent;
SDL_Window* window;
SDL_GLContext glContext;

Text* tt;
Image* im;
Model* md;

int Scence::MainWorld()
{
    // ��ʼ��SDL
    SDL_Init(SDL_INIT_EVERYTHING);
   
    // ����OpenGL�����İ汾
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // ����SDL����
    window = SDL_CreateWindow(" ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* render = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(render, &rendererInfo);
    if (strncmp(rendererInfo.name, "opengl", 6)) std::cout << "88888" << std::endl;;

    // ����OpenGL������
    SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(0);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    // ��ʼ��GLEW
    glewExperimental = GL_TRUE; // ����GLEW����չ֧��
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // ��ӡOpenGL�汾
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    // ���ô���ͼ��
    SDL_Surface* icon = im->LoadICON("logo.ico");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else {
        std::cerr << "Failed to load icon image." << std::endl;
    }

    // ����������ģ��
    md = new Model();
    if (md == nullptr) {
        std::cerr << "Failed to create Model instance." << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::string npc173 = "GFX/NPC/173/npc173.obj";
    const char* tex173 = "GFX/NPC/173/173texture.jpg";
    if (md->LoadOBJ(npc173,tex173) != 0) {
        std::cerr << "Failed to load OBJ file." << std::endl;
        delete md;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // ��ѭ��
    bool running = true;
    while (running)
    {
        // �����¼�
        while (SDL_PollEvent(&mainevent) != 0)
        {
            switch (mainevent.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (mainevent.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (mainevent.button.button == SDL_BUTTON_LEFT)
                {
                    // Handle mouse left button press
                }
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //tt->Draw_Text2(render,"C:/Windows/Fonts/simfang.ttf",20,20,20,L"173",255,255,255,255);
        //SDL_GL_ActivateRenderer(render);

        OpenGLText glt;
        glt.Main();

        // ��Ⱦģ��
        md->render();
        
        // �������ڻ���������ʾ��Ⱦ������
        SDL_GL_SwapWindow(window);
        //SDL_RenderPresent(render);
    }

    // ������Դ
    delete md;
    SDL_DestroyRenderer(render);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

