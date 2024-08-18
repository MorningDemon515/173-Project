
#include "Text.h"

#include "UTF8.h"

#include <codecvt>

#include "Text_GL.h"

FT_Library ft;
FT_Face face;

UTF8* utf8;

SDL_Texture* texture;

const char* VERTEX_SHADER = ""
"#version 410 core\n"
"in vec4 in_Position;\n"
"out vec2 texCoords;\n"
"void main(void) {\n"
"    gl_Position = vec4(in_Position.xy, 0, 1);\n"
"    texCoords = in_Position.zw;\n"
"}\n";


const char* FRAGMENT_SHADER = ""
"#version 410 core\n"
"precision highp float;\n"
"uniform sampler2D tex;\n"
"uniform vec4 color;\n"
"in vec2 texCoords;\n"
"out vec4 fragColor;\n"
"void main(void) {\n"
"    fragColor = vec4(1, 1, 1, texture(tex, texCoords).r) * color;\n"
"}\n";

int Text::utf8_to_unicode(const char* utf8, wchar_t* unicode)
{
    mbstate_t state;
    memset(&state, 0, sizeof(state));
    const char* src = utf8;
    wchar_t* dst = unicode;
    size_t len = mbsrtowcs(dst, &src, strlen(utf8), &state);
    return len;
}

void Text::render_text(SDL_Renderer* renderer, FT_Face face, const wchar_t* text, int x, int y,int r,int gv,int b)
{
    int pen_x = x;
    int pen_y = y;
    for (const wchar_t* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            fprintf(stderr, "Could not load character '%lc'\n", *p);
            continue;
        }

        FT_GlyphSlot g = face->glyph;

        // Handle space character manually
        if (*p == L' ') {
            pen_x += g->advance.x >> 6;
            continue;
        }

        // Create an SDL texture to hold the glyph bitmap
        texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            g->bitmap.width,
            g->bitmap.rows);

        if (!texture) {
            //fprintf(stderr, "Could not create texture: %s\n", SDL_GetError());
            continue;
        }

        // Enable blending mode for transparency
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        // Copy glyph bitmap to texture
        Uint32* pixels = NULL;
        int pitch = 0;
        SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
        for (int y = 0; y < g->bitmap.rows; y++) {
            for (int x = 0; x < g->bitmap.width; x++) {
                Uint8 value = g->bitmap.buffer[y * g->bitmap.width + x];
                pixels[y * (pitch / 4) + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), r, gv, b, value);
            }
        }
        SDL_UnlockTexture(texture);

        // Calculate destination rectangle
        SDL_Rect dstrect = { pen_x + g->bitmap_left, pen_y - g->bitmap_top, g->bitmap.width, g->bitmap.rows };

        // Render texture
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        // Advance the pen position
        pen_x += g->advance.x >> 6;

        // Cleanup texture
        SDL_DestroyTexture(texture);
    }
}

int Text::Draw_Text(SDL_Renderer* renderer, const char* font, int x, int y, int size, const char* text, int r, int g, int b, int a)
{
    setlocale(LC_ALL, "");

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, font, 0, &face);
    FT_Set_Pixel_Sizes(face, 0, size);

    const char* text_utf8 = text;
    wchar_t text_unicode[256];
    utf8_to_unicode(text_utf8, text_unicode);

    //SDL_SetRenderDrawColor(renderer, r, g, b, a);
    //SDL_RenderClear(renderer);
    //text_unicode
    render_text(renderer, face, text_unicode, x, y,r,g,b);//WINDOW_WIDTH/2, WINDOW_HEIGHT/2

    //SDL_RenderPresent(renderer);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

int Text::Draw_Text2(SDL_Renderer* renderer, const char* font, int x, int y, int size, const wchar_t* text, int r, int g, int b, int a)
{
    setlocale(LC_ALL, "");

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, font, 0, &face);
    FT_Set_Pixel_Sizes(face, 0, size);

    //const char* text_utf8 = text;
    //wchar_t text_unicode[256];
    //utf8_to_unicode(text_utf8, text_unicode);

    //SDL_SetRenderDrawColor(renderer, r, g, b, a);
    //SDL_RenderClear(renderer);
    //text_unicode
    render_text(renderer, face, text, x, y, r, g, b);//WINDOW_WIDTH/2, WINDOW_HEIGHT/2

    //SDL_RenderPresent(renderer);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

/*
GLuint tex{ 0 }, sam{ 0 };
GLuint vbo{ 0 }, vao{ 0 };
GLuint vs{ 0 }, fs{ 0 }, program{ 0 };

void Text::render_text(const std::string& str, FT_Face face, float x, float y, float sx, float sy)
{
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    const FT_GlyphSlot glyph = face->glyph;

    for (auto c : str) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
            continue;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
            glyph->bitmap.width, glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        const float vx = x + glyph->bitmap_left * sx;
        const float vy = y + glyph->bitmap_top * sy;
        const float w = glyph->bitmap.width * sx;
        const float h = glyph->bitmap.rows * sy;

        struct {
            float x, y, s, t;
        } data[6] = {
            {vx    , vy    , 0, 0},
            {vx    , vy - h, 0, 1},
            {vx + w, vy    , 1, 0},
            {vx + w, vy    , 1, 0},
            {vx    , vy - h, 0, 1},
            {vx + w, vy - h, 1, 1}
        };

        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), data, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (glyph->advance.x >> 6) * sx;
        y += (glyph->advance.y >> 6) * sy;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //-----------------------------------------------------------------

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    const FT_GlyphSlot glyph = face->glyph;

    for (int i = 0; i < str.size(); ) {
        int unicodeChar = UTF8::decodeCharacter(str.c_str(), i);
        i += UTF8::measureCodepoint(str[i]);

        if (FT_Load_Char(face, unicodeChar, FT_LOAD_RENDER) != 0)
            continue;

        // Bind the texture before updating it
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, glyph->bitmap.width, glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const float vx = x + glyph->bitmap_left * sx;
        const float vy = y + glyph->bitmap_top * sy;
        const float w = glyph->bitmap.width * sx;
        const float h = glyph->bitmap.rows * sy;

        struct {
            float x, y, s, t;
        } data[6] = {
            {vx    , vy    , 0, 0},
            {vx    , vy - h, 0, 1},
            {vx + w, vy    , 1, 0},
            {vx + w, vy    , 1, 0},
            {vx    , vy - h, 0, 1},
            {vx + w, vy - h, 1, 1}
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (glyph->advance.x >> 6) * sx;
        y += (glyph->advance.y >> 6) * sy;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
}

int Text::RenderText()
{
    
    FT_Init_FreeType(&ft);
    FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face);

    const int WIDTH = 1280;
    const int HEIGHT = 720;
    const double SCALEX = 2.0 / WIDTH;
    const double SCALEY = 2.0 / HEIGHT;

    // Initialize our texture and VBOs
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenTextures(1, &tex);
    glGenSamplers(1, &sam);
    glSamplerParameteri(sam, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sam, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sam, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sam, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Initialize shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VERTEX_SHADER, 0);
    glCompileShader(vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FRAGMENT_SHADER, 0);
    glCompileShader(fs);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Set some initialize GL state
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glClearColor(0.1, 0.2, 0.4, 0);

    // Get shader uniforms
    glUseProgram(program);
    glBindAttribLocation(program, 0, "in_Position");
    GLuint texUniform = glGetUniformLocation(program, "tex");
    GLuint colorUniform = glGetUniformLocation(program, "color");

    // Bind stuff
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindSampler(0, sam);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(program);
    glUniform4f(colorUniform, 1, 1, 1, 1);
    glUniform1i(texUniform, 0);

    FT_Set_Pixel_Sizes(face, 0, 50);
    render_text("OpenGL中文测试!", face, 0, 0, SCALEX, SCALEY);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glDeleteTextures(1, &tex);
    glDeleteSamplers(1, &sam);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);

    return 0;
    
}*/

/*
int TextGL::Draw_Text_GL()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned int c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);


    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    RenderText(shader, "文本", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

    return 0;

}*/





















