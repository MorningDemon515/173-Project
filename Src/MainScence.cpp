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

Image_GL* imgl;
Image* image;
Model* mdl;

SDL_Event e;//SDL事件
SDL_GLContext GLContext;//OpenGL上下文

int Scence::MainWorld()
{
    SDL_Window* window = SDL_CreateWindow(" ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);//创建窗口

    //设置窗口标题和图标
    SDL_Surface* icon = image->LoadICON("logo.ico");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_SetWindowTitle(window, "SCP-Containment Breach");

    GLContext = SDL_GL_CreateContext(window);//创建OpenGL上下文
    SDL_GL_MakeCurrent(window, GLContext);//设置OpenGL上下文
    //SDL_GL_SetSwapInterval(1); // 垂直同步

    glewExperimental = GL_TRUE;//初始化GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //输入173纹理和模型
    std::string npc173 = "GFX/NPC/173/npc173.obj";
    const char* tex173 = "GFX/NPC/173/173texture.jpg";

    //加载模型
    mdl->LoadOBJ(npc173,tex173);

    SpriteRenderer spriteRenderer;
    spriteRenderer.Init();
    spriteRenderer.LoadTexture("GFX/menu/back.png");


    //设置主循环
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
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清屏

        //imgl->Draw_Image_GL();

        //glEnable(GL_DEPTH_TEST);//开启深度测试
        //mdl->render();//渲染模型

        //SDL_GL_SwapWindow(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the sprite
        spriteRenderer.DrawSprite(glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, -0.5f), 0.0f);

        SDL_GL_SwapWindow(window);


        SDL_Delay(1);

    }

    mdl->cleanup();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
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
    // 初始化SDL
    SDL_Init(SDL_INIT_EVERYTHING);
   
    // 设置OpenGL上下文版本
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 创建SDL窗口
    window = SDL_CreateWindow(" ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* render = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(render, &rendererInfo);
    if (strncmp(rendererInfo.name, "opengl", 6)) std::cout << "88888" << std::endl;;

    // 创建OpenGL上下文
    SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(0);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    // 初始化GLEW
    glewExperimental = GL_TRUE; // 启用GLEW的扩展支持
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 打印OpenGL版本
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    // 设置窗口图标
    SDL_Surface* icon = im->LoadICON("logo.ico");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else {
        std::cerr << "Failed to load icon image." << std::endl;
    }

    // 创建并加载模型
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

    

    // 主循环
    bool running = true;
    while (running)
    {
        // 处理事件
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
        // 渲染模型
        //md->render();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 1280, 720);

        imgl->Draw_Image_GL();

        SDL_GL_SwapWindow(window);

        // 交换窗口缓冲区，显示渲染的内容
        //SDL_GL_SwapWindow(window);
        //SDL_RenderPresent(render);
    
    }

    // 清理资源

    delete md;
    SDL_DestroyRenderer(render);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/

// Vertex and Fragment Shader Source Code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec2 aTexCoord;
    
    out vec2 TexCoord;
    
    uniform mat4 model;
    
    void main()
    {
        gl_Position = model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    
    in vec2 TexCoord;
    
    uniform sampler2D texture1;
    
    void main()
    {
        FragColor = texture(texture1, TexCoord);
    }
)";

SpriteRenderer::SpriteRenderer() : textureID(0), VAO(0), VBO(0), shaderProgram(0), textureWidth(0), textureHeight(0), nrChannels(0) {}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureID);
    glDeleteProgram(shaderProgram);
}

bool SpriteRenderer::Init() {
    // 编译着色器并链接程序
    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    shaderProgram = LinkProgram(vertexShader, fragmentShader);

    // 设置四边形的 VAO 和 VBO
    SetupQuad();

    return true;
}

void SpriteRenderer::LoadTexture(const std::string& filePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 加载纹理数据
    unsigned char* data = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }
    stbi_image_free(data);
}

void SpriteRenderer::DrawSprite(const glm::vec2& position, const glm::vec2& size, float rotation) {
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLuint textureLoc = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(textureLoc, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::SetupQuad() {
    float vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top-right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom-right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left
       -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // Top-left
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f  // Top-right
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

GLuint SpriteRenderer::CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

GLuint SpriteRenderer::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Program linking error: " << infoLog << std::endl;
    }
    return program;
}



