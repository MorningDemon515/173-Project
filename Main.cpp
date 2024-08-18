
#include "Src\MainScence.h"
#include "Src\Launcher.h"

Menu* menu;
Scence* sce;

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	//menu->Launcher();
	sce->MainWorld();
	return 0;
}