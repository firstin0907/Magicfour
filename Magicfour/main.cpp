#include "header.h"
#include "App.h"

int main(int args, char* argv[]) {

	try
	{
		App app = App("¸¶¹ý4", Vector2d<Uint32>(1280, 720));
		app.run();
	}
	catch (std::exception e)
	{
		std::string error_message = e.what();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.what(),
			(SDL_GetError() + std::string(" ")).c_str(), NULL);
	}
	
	return 0;
}