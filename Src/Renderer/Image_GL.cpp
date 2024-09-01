
//#define STB_IMAGE_IMPLEMENTATION
#include "Image_GL.h"
#include "RenderSprite.h"

#include <gl2d\gl2d.h>

#include <stb_image.h>

OUTAPI(int) Image_GL::Draw_BackGround_GL(const char* filepath)
{
    int image_w, image_h,channel;

    unsigned char* idata = stbi_load(filepath, &image_w, &image_h, &channel, 0);

    gl2d::init();
    gl2d::Renderer2D renderer;
    renderer.create();

    gl2d::Texture texture(filepath);

    renderer.updateWindowMetrics(1280, 720);

    renderer.renderRectangle({ 0, 0, image_w, image_h }, texture, Colors_White, {}, 0, glm::vec4{0,1,1,0});
  
    renderer.flush();

    return 0;
}


