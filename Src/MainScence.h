#ifndef MAINSCENCE_H
#define MAINSCENCE_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SDL.h>
#include <string>

class Scence
{

public:
	int MainWorld();
};

//==================================================
class SpriteRenderer {
public:
    SpriteRenderer();
    ~SpriteRenderer();

    bool Init();
    void LoadTexture(const std::string& filePath);
    void DrawSprite(const glm::vec2& position, const glm::vec2& size, float rotation);

private:
    void SetupQuad();
    GLuint CompileShader(const char* source, GLenum type);
    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

    GLuint textureID;
    GLuint VAO, VBO;
    GLuint shaderProgram;

    int textureWidth;  // 保存纹理的宽度
    int textureHeight; // 保存纹理的高度
    int nrChannels;
};





#endif // !MAINSCENCE_H
