#include "Text_GL.h"

#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <windows.h>

#include "FileSystem.h"

FileSys* fs;

void OpenGLText::InitFreeType(const std::wstring& t) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        return;
    }

    if (FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    LoadCharacters(face, t); // Pass the text to load characters

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLText::LoadCharacters(FT_Face& face, const std::wstring& t) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (wchar_t c : t) { // Use the actual text characters
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters[c] = character; // Use the actual character as the key
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

int OpenGLText::Main() {

    //��Ⱦ�ı���������
    std::wstring text = L"\u4f60\u597d\u4e16\u754c"; // Chinese for "Hello World"//����Ҫ��Ⱦ���ı�
    //XY��
    int x, y;
    x = fs->ReadINIint("option.ini","axle","xaxle");
    y = fs->ReadINIint("option.ini", "axle","yaxle");
    //��ʼ��
    InitFreeType(text); // Pass the text to initialize FreeType
    //����
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //��ɫ��
    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");
    //�ӿ�
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));//�ӿ�
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Render text
    RenderText(shader, text, (float)x, (float)y, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));//��Ⱦ

    return 0;
}

void OpenGLText::RenderText(Shader& shader, const std::wstring& text, float x, float y, float scale, glm::vec3 color) {
    glUseProgram(shader.ID);
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (wchar_t c : text) {
        auto it = Characters.find(c);
        if (it == Characters.end()) {
            continue; // Skip characters that are not loaded
        }
        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/*
void OpenGLText::InitFreeType(std::wstring t) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        return;
    }

    if (FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    LoadCharacters(face,t);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void OpenGLText::LoadCharacters(FT_Face& face, std::wstring& t) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Limit the range of characters to load to a practical subset.
    //: {L'\u4f60', L'\u597d', L'\u4e16', L'\u754c'}
    for (wchar_t c : {*t.c_str()}) { // Load only the characters you need
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters[c] = character; // Use the actual character as the key
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

int OpenGLText::Main() {
    std::wstring text = L"\u4f60\u597d\u4e16\u754c";

    InitFreeType(text);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Render text
    RenderText(shader, text, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    return 0;
}

void OpenGLText::RenderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color) {
    glUseProgram(shader.ID);
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (auto& c : text) {
        auto it = Characters.find(c);
        if (it == Characters.end()) {
            continue; // Skip characters that are not loaded
        }
        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
*/
//----------------------------------------------------------

/*
void OpenGLText::InitFreeType() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    LoadCharacters(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLText::LoadCharacters(FT_Face& face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (wchar_t c = 0; c<65535; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<wchar_t, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

int OpenGLText::Main() {
    InitFreeType();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // �����Ѷ���shader������ʼ����OpenGL����
    RenderText(shader, L"\u4f60\u597d\u4e16\u754c", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    return 0;
}

void OpenGLText::RenderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color) {
    glUseProgram(shader.ID);
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (auto& c : text) {
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
*/

//------------------------------------------------------------------------------------------
/*
// ��Ⱦ�ı��ĺ���
void TextGL::RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // �����Ӧ����Ⱦ״̬	
    shader.use();
    // �����ı���ɫ
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    // ��������Ԫ0
    glActiveTexture(GL_TEXTURE0);
    // ��VAO
    glBindVertexArray(VAO);

    // ���������ַ�
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        // ��Characters�ֵ��л�ȡ��ǰ�ַ�����Ϣ
        Character ch = Characters[*c];

        // �����ַ�����Ļ�ϵ�λ��
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        // �����ַ��Ŀ�Ⱥ͸߶�
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // ����VBO�Ķ������ݣ���������ַ����ı���
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // �󶨵�ǰ�ַ�������
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // ����VBO�ڴ������
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // ע��ʹ��glBufferSubData������glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // �����ַ���Ӧ���ı���
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // �ƶ���굽��һ���ַ���λ�ã�ע��Advance��1/64���ص�����
        x += (ch.Advance >> 6) * scale; // ����6λ���൱�ڳ���64������λת��Ϊ����
    }
    // ���VAO������
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ��ʼ��OpenGL��FreeType������Ⱦ�ı��ĺ���
int TextGL::Draw_Text_GL()
{
    // ����OpenGL�����޳��ͻ�Ϲ���
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ������ʹ����ɫ������
    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");

    // ��������ͶӰ��������2D�ı���Ⱦ
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // ��ʼ��FreeType��
    FT_Init_FreeType(&ft);
    // �������壬����ʹ��Windowsϵͳ�еķ�������
    FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face);

    // ������������ش�С
    FT_Set_Pixel_Sizes(face, 0, 48);

    // �����ֽڶ������ƣ���1�ֽڶ��룩
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // ����ASCII�ַ�����0��127��
    for (unsigned int c = 0; c < 128; c++)
    {
        // �����ַ�������
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // ��������
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

        // ��������ѡ��
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ���ַ�����Ϣ�洢��Characters�ֵ��У��Ա����ʹ��
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    // �������
    glBindTexture(GL_TEXTURE_2D, 0);

    // ����FreeType��Դ
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // ����VAO��VBO���Ա���������ַ�ʱʹ��
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ��Ⱦʾ���ı�
    RenderText(shader, "This is a Simple Text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

    return 0;
}*/

/*
// ���ı��ĺ���
void TextGL::RenderText(Shader& shader, std::wstring text, float x, float y, float scale, glm::vec3 color)
{
    // �����Ӧ����Ⱦ״̬	
    shader.use();
    // �����ı���ɫ
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    // ��������Ԫ0
    glActiveTexture(GL_TEXTURE0);
    // ��VAO
    glBindVertexArray(VAO);

    // ���������ַ�
    std::wstring::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        // ��Characters�ֵ��л�ȡ��ǰ�ַ�����Ϣ
        Character ch = Characters[*c];

        // �����ַ�����Ļ�ϵ�λ��
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        // �����ַ��Ŀ�Ⱥ͸߶�
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // ����VBO�Ķ������ݣ���������ַ����ı���
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // �󶨵�ǰ�ַ�������
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // ����VBO�ڴ������
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // ע��ʹ��glBufferSubData������glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // �����ַ���Ӧ���ı���
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // �ƶ���굽��һ���ַ���λ�ã�ע��Advance��1/64���ص�����
        x += (ch.Advance >> 6) * scale; // ����6λ���൱�ڳ���64������λת��Ϊ����
    }
    // ���VAO������
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ��ʼ��OpenGL��FreeType������Ⱦ�ı��ĺ���
int TextGL::Draw_Text_GL()
{
    setlocale(LC_ALL, "");

    // ����OpenGL�����޳��ͻ�Ϲ���
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ������ʹ����ɫ������
    Shader shader("GFX/GLSL/text.vs", "GFX/GLSL/text.fs");

    // ��������ͶӰ��������2D�ı���Ⱦ
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // ��ʼ��FreeType��
    FT_Init_FreeType(&ft);
    // �������壬����ʹ��Windowsϵͳ�еķ�������
    FT_New_Face(ft, "C:/Windows/Fonts/simfang.ttf", 0, &face);

    // ������������ش�С
    FT_Set_Pixel_Sizes(face, 0, 48);

    // �����ֽڶ������ƣ���1�ֽڶ��룩
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    TCHAR wenb[] = L"���GL!";
    // ����ASCII�ַ�����0��127��
    for (TCHAR* c = wenb; *c ; c++)
    {

        // �����ַ�������
        if (FT_Load_Char(face, *c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // ��������
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

        // ��������ѡ��
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ���ַ�����Ϣ�洢��Characters�ֵ��У��Ա����ʹ��
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<wchar_t, Character>(*c, character));
    }

    // �������
    glBindTexture(GL_TEXTURE_2D, 0);

    // ����FreeType��Դ
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // ����VAO��VBO���Ա���������ַ�ʱʹ��
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ��Ⱦʾ���ı�
    RenderText(shader, wenb, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    //RenderText(shader, L"(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

    return 0;
}*/



