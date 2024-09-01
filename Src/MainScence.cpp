#include "MainScence.h"
#include "LoadModel.h"
#include "Launcher.h"

#include <SDL_opengl.h>
#include <SDL_Video.h>
#include <iostream>

#include <stb_image.h>

#include "Image.h"
#include "Text.h"

#include "Text_GL.h"

#include "Shader.h"

#include "Renderer/Image_GL.h"

#include <windows.h>

Image_GL* igl;
Image* image;
Model* mdl;

SDL_Event e;//SDL�¼�
SDL_GLContext GLContext;//OpenGL������

int Scence::MainWorld()
{
    SDL_Window* window = SDL_CreateWindow(" ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);//��������

    //���ô��ڱ����ͼ��
    SDL_Surface* icon = image->LoadICON("logo.ico");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_SetWindowTitle(window, "SCP-Containment Breach");

    GLContext = SDL_GL_CreateContext(window);//����OpenGL������
    SDL_GL_MakeCurrent(window, GLContext);//����OpenGL������
    //SDL_GL_SetSwapInterval(1); // ��ֱͬ��

    glewExperimental = GL_TRUE;//��ʼ��GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //����173�����ģ��
    std::string npc173 = "GFX/NPC/173/npc173.obj";
    const char* tex173 = "GFX/NPC/173/173texture.jpg";

    //����ģ��
    mdl->LoadOBJ(npc173,tex173);

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
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//����

        //imgl->Draw_Image_GL();

        //glEnable(GL_DEPTH_TEST);//������Ȳ���
        //mdl->render();//��Ⱦģ��

        //SDL_GL_SwapWindow(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);

        igl->Draw_BackGround_GL("GFX/menu/Launcher.jpg");

        SDL_Delay(1);

    }

    mdl->cleanup();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// ���������ļ���ȡ��ɫ������
std::string Scence::readShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// ������������ɫ��
GLuint Scence::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }
    return shader;
}

// ������������ɫ������
GLuint Scence::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    // ��ȡ��ɫ������
    std::string vertexCode = readShaderSource(vertexPath);
    std::string fragmentCode = readShaderSource(fragmentPath);

    // ���붥����ɫ����Ƭ����ɫ��
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // ������ɫ������
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    // ɾ����ɫ������������ɺ�����Ҫ
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

/*
SDL_Event mainevent;
SDL_Window* window;
SDL_GLContext glContext;

Text* tt;
Image* im;
Model* md;
Image_GL* imgl;

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

    //tt->Draw_Text2(render,"C:/Windows/Fonts/simfang.ttf",20,20,20,L"173",255,255,255,255);
    //SDL_GL_ActivateRenderer(render);

    

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
        //glClearColor(255, 255, 255, 255);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //OpenGLText glt;
        //glt.Main();
        // ��Ⱦģ��
        //md->render();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 1280, 720);

        imgl->Draw_Image_GL();

        SDL_GL_SwapWindow(window);

        // �������ڻ���������ʾ��Ⱦ������
        //SDL_GL_SwapWindow(window);
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
*/





