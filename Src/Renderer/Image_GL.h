#ifndef IMAGE_GL_H
#define IMAGE_GL_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#ifdef _MSC_VER
#define OUTAPI(x) __declspec(dllexport) x
#else
#define OUTAPI(x) extern "C" x
#endif

#include <GL/glew.h>
#include <iostream>
#include <map>


class Image_GL
{
private:
    
public:
    OUTAPI(int) Draw_BackGround_GL(const char* filepath);
};






#endif // !IMAGE_GL_H
