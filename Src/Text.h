#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>

#include <GL\glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>

#include <wchar.h>
#include <locale.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Text
{
private:
	int utf8_to_unicode(const char* utf8, wchar_t* unicode);
	void render_text(SDL_Renderer* renderer, FT_Face face, const wchar_t* text, int x, int y, int r, int g, int b);

	void render_text(const std::string& str, FT_Face face, float x, float y, float sx, float sy);

public:
	int Draw_Text(SDL_Renderer* renderer,const char* font,int x,int y,int size, const char* text,int r,int g,int b,int a);
	int Draw_Text2(SDL_Renderer* renderer, const char* font, int x, int y, int size, const wchar_t* text, int r, int g, int b, int a);
	int RenderText();

};




#endif // !TEXT_H

