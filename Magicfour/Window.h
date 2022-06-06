#pragma once
#include "header.h"
#include "utility.h"

class Window
{
	SDL_Window*			window_;

	std::string			title_;
	Vector2d<Uint32>	window_size_;

public:

	operator SDL_Window*() {
		return window_;
	}

	Window(std::string title, Vector2d<Uint32> window_size, std::string icon_file_name = "icon.png")
		: title_(title), window_size_(window_size)
	{
		window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, window_size.x, window_size.y, SDL_WINDOW_SHOWN);

		if (window_ == nullptr) {
			throw std::exception("Failed to create window");
		}

		// Window의 아이콘을 설정한다.
		SDL_Surface* icon_surface = IMG_Load(icon_file_name.c_str());
		if (icon_surface != nullptr) {
			SDL_SetWindowIcon(window_, icon_surface);
			SDL_FreeSurface(icon_surface);
			icon_surface = nullptr;
		}
	};

	~Window()
	{
		SDL_DestroyWindow(window_);
	};
};

