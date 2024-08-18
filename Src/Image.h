#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>

class Image
{
private:
	
public:
	int DrawImage(SDL_Renderer* rend,const char* file,int x,int y);
	SDL_Surface* LoadICON(const char* icon);
};




#endif // !IMAGE_H
