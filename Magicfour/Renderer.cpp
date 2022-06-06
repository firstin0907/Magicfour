#include "Renderer.h"
#include "global.h"


Renderer::Renderer(SDL_Window* window, Vector2d<Uint32> logical_size)
	: logical_size_(logical_size)
{
	renderer_ = SDL_CreateRenderer(window, -1, 0);
	if (renderer_ == nullptr)
	{
		throw std::exception("Failed to create renderer. ");
	}

	if (logical_size.x > 0)
	{
		SDL_RenderSetLogicalSize(renderer_, logical_size.x, logical_size.y);
	}

	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

	std::string suffix = "Resource\\Graphic\\";

	for (int i = 0; i < 10; i++)
	{
		texture_['U' - 'A'].emplace_back(
			suffix + 'U' + std::to_string(i) + ".ma4png",
			suffix + 'U' + std::to_string(i) + ".txt",
			renderer_);
	}
	for (int i = 0; i < 5; i++)
	{
		texture_['C' - 'A'].emplace_back(
			suffix + 'C' + std::to_string(i) + ".ma4png",
			suffix + 'C' + std::to_string(i) + ".txt",
			renderer_);
	}
	for (int i = 0; i < 9; i++)
	{
		texture_['M' - 'A'].emplace_back(
			suffix + 'M' + std::to_string(i) + ".ma4png",
			suffix + 'M' + std::to_string(i) + ".txt",
			renderer_);
	}
	for (int i = 0; i < 7; i++)
	{
		texture_['O' - 'A'].emplace_back(
			suffix + 'O' + std::to_string(i) + ".ma4png",
			suffix + 'O' + std::to_string(i) + ".txt",
			renderer_);
	}

	for (int i = 0; i < 1; i++)
	{
		std::string file_name = "Resource\\Graphic\\Font\\" + std::to_string(i) + ".ttf";
		font_.push_back(TTF_OpenFont(file_name.c_str(), 24));
		font_.push_back(TTF_OpenFont(file_name.c_str(), 18));
	}
}

void Renderer::clear()
{
	SDL_RenderClear(renderer_);
}

void Renderer::set_color(Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Renderer::present()
{
	SDL_RenderPresent(renderer_);
}

void Renderer::draw_range(const Range<int>& range)
{
	switch (range.type)
	{
	case Range<int>::RANGE_CIRCLE:
		for (int x = -range.r; x <= range.r; x++)
		{
			int a = range.r * range.r - (x - 1) * (x - 1);
			int b = range.r * range.r - (x + 1) * (x + 1);

			if (a < 0) a = 0;
			if (b < 0) b = 0;
			a = static_cast<int>(sqrt(a)), b = static_cast<int>(sqrt(b));
			SDL_RenderDrawLine(renderer_, range.center.x + x,
				range.center.y + a, range.center.x + x, range.center.y + b);
			SDL_RenderDrawLine(renderer_, range.center.x + x,
				range.center.y - a, range.center.x + x, range.center.y - b);
		}
		break;
	case Range<int>::RANGE_RECT:
		SDL_RenderDrawRect(renderer_, &static_cast<SDL_Rect>(range));
		break;
	default:
		throw std::exception("Unknown range type error");
	}
}

void Renderer::draw_rect(const Range<int>& range)
{
	SDL_RenderFillRect(renderer_, &static_cast<SDL_Rect>(range));
}

Texture& Renderer::get_texture(char alpha, int id)
{
	return texture_[alpha][id];
}

void Renderer::enroll_texture(std::weak_ptr<Texture> texture,
	Point<int> point, int section, SDL_RendererFlip flip)
{
	texture.lock()->draw(renderer_, point, section, flip);
}

void Renderer::enroll_texture(char alpha, int id, Point<int> point, int section, SDL_RendererFlip flip)
{
	alpha -= 'A';
	if (id < texture_[alpha].size())
	{
		texture_[alpha][id].draw(renderer_, point, section, flip);
	}
}

void Renderer::enroll_texture(char alpha, int id, Range<int> range, int section, SDL_RendererFlip flip)
{
	alpha -= 'A';
	if (id < texture_[alpha].size())
	{
		texture_[alpha][id].draw(renderer_, range, section, flip);
	}
}

void Renderer::enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect)
{
	alpha -= 'A';
	if (id < texture_[alpha].size())
	{
		SDL_RenderCopy(renderer_, texture_[alpha][id],
			&static_cast<SDL_Rect>(src_rect), &static_cast<SDL_Rect>(dst_rect));
	}
}


void Renderer::enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect,
	int angle, Point<int> center, SDL_RendererFlip flip)
{
	alpha -= 'A';
	if (id < texture_[alpha].size())
	{
		SDL_RenderCopyEx(renderer_, texture_[alpha][id],
			&static_cast<SDL_Rect>(src_rect), &static_cast<SDL_Rect>(dst_rect), angle,
			&static_cast<SDL_Point>(center), flip);
	}
}

std::shared_ptr<Texture> Renderer::make_text(std::string message, int font_num, SDL_Color color)
{
	return std::make_shared<Texture>(message, renderer_, font_[font_num % font_.size()], color);
}

Renderer::operator SDL_Renderer*&()
{
	return renderer_;
}

Renderer::~Renderer()
{
	for (auto &i : font_) TTF_CloseFont(i);
	SDL_DestroyRenderer(renderer_);
}
