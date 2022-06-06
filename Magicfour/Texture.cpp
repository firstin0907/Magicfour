#include "Texture.h"

Texture::Texture(std::string img_file_name, std::string data_file_name, SDL_Renderer* renderer)
	: texture(nullptr)
{
	SDL_Surface *surface = IMG_Load(img_file_name.c_str());
	if (surface == nullptr)
	{
		throw std::exception("Fail to load image");
	}

	size.x = surface->w, size.y = surface->h;

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	if (texture == nullptr)
	{
		throw std::exception("Fail to create texture");
	}

	std::ifstream data_file(data_file_name);

	if (!(data_file >> section_num)) section_num = 1;
	if (!(data_file >> center.x)) center.x = 0;
	if (!(data_file >> center.y)) center.y = 0;

	size.x /= section_num;
}

Texture::Texture(std::string message, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color)
{
	if (renderer == nullptr)
		throw std::exception("Fail to create text texture because of SDL_Renderer");
	if (font == nullptr)
		throw std::exception("Fail to create text texture because of TTF_Font");
	
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, message.c_str(), color);

	if (surface == nullptr)
		throw std::exception("Fail to create text texture because of SDL_surface");

	size.x = surface->w, size.y = surface->h;

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	if (texture == nullptr)
		throw std::exception("Fail to create text texture");

	section_num = 1;
	center.x = center.y = 0;
}

Texture::Texture(Texture && t) noexcept
{
	texture = t.texture;
	size = t.size;
	center = t.center;
	section_num = t.section_num;

	t.texture = nullptr;
}


void Texture::draw(SDL_Renderer* renderer, Point<int> point, int section, SDL_RendererFlip flip)
{
	section %= section_num;

	SDL_Rect src = { size.x * section, 0, size.x, size.y };
	SDL_Rect dst = { point.x - center.x, point.y - center.y, size.x, size.y };

	if (flip & SDL_FLIP_HORIZONTAL)
	{
		dst = { point.x - size.x + center.x, point.y - center.y, size.x, size.y };
	}

	SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, nullptr, flip);
}

void Texture::draw(SDL_Renderer* renderer, Range<int> range, int section, SDL_RendererFlip flip)
{
	section %= section_num;

	SDL_Rect src = { size.x * section, 0, size.x, size.y };
	SDL_Rect dst = range;

	SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, nullptr, flip);
}


Texture::operator SDL_Texture*()
{
	return texture;
}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}
