#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <SDL.h>
#include <iostream>
#include <string>

class Menu
{
private:
	void Button(SDL_Renderer* rend, int x, int y, int w, int h, std::string t);
	void Button2(SDL_Renderer* rend, int x, int y, int w, int h, const wchar_t * t);
	bool IsDown(SDL_Rect rectt);
	int PrintDevInf(SDL_Renderer* rend, int x, int y);
	int SelectDriver(SDL_Renderer* rend);
	int SelectR(SDL_Renderer* rend);

public:
	int Launcher();
	
};


#endif // !LAUNCHER_H
