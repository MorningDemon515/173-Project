#ifndef TEXT_GL_H
#define TEXT_GL_H

#include <iostream>
#include <map>
#include <string>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

/*
class TextGL
{
private:
    
    
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;

    FT_Library ft;
    FT_Face face;
    


public:

	void RenderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color);

    int Draw_Text_GL();


};*/


// 假设有Shader类已定义
struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class OpenGLText {
public:
    std::map<wchar_t, Character> Characters;
    GLuint VAO, VBO;

    FT_Library ft;
    FT_Face face;

    int Main();
    void RenderText(Shader& shader, const std::wstring& text, float x, float y, float scale, glm::vec3 color);

private:
    void InitFreeType(const std::wstring& t);
    void LoadCharacters(FT_Face& face, const std::wstring& t);
};


/*
struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class OpenGLText {
public:
    std::map<wchar_t, Character> Characters;
    GLuint VAO, VBO;

    int Main();
    void RenderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color);

private:
    void InitFreeType();
    Character LoadCharacter(wchar_t c, FT_Face& face);
};
*/









#endif // !TEXT_GL_H
