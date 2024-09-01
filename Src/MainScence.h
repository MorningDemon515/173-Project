#ifndef MAINSCENCE_H
#define MAINSCENCE_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SDL.h>
#include <string>
#include <vector>

class Scence
{

public:
	int MainWorld();

    std::string readShaderSource(const std::string& filepath);
    GLuint compileShader(GLenum type, const std::string& source);
    GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
};

//==================================================

#endif // !MAINSCENCE_H
