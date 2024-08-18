
#include "Src\MainScence.h"
#include "Src\Launcher.h"
#include <windows.h>

Menu* menu;
Scence* sce;

int main(int argc, char* args[])
{

	GetModuleHandle(L"bin/assimp-vc142-mtd.dll");

	SDL_Init(SDL_INIT_EVERYTHING);

	menu->Launcher();
	//sce->MainWorld();
	return 0;
}