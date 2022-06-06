#pragma once
#include "header.h"

struct Texture
{
	SDL_Texture*	texture;
	Vector2d<int>	size;

	Point<int>		center;
	int				section_num;


public:
	// �̹��� ������ �̿��� texture �����!
	Texture(std::string img_file_name, std::string data_file_name,
		SDL_Renderer* renderer);

	// font�� �̿��� texture �����!
	Texture(std::string message, SDL_Renderer* renderer, TTF_Font* font,
		SDL_Color color = { 255, 255, 255, 255 });

	// move ������
	Texture(Texture&& t) noexcept;

	// renderer�� texture�� �߽�(center)�� point�� ���Բ� �׸���. ��µǴ� ũ��� �״�δ�.
	void draw(SDL_Renderer* renderer, Point<int> point, int section, SDL_RendererFlip flip);

	// renderer�� range �κп� �׸���. ���� ��µǴ� ũ�Ⱑ �޶��� �� �ִ�.
	void draw(SDL_Renderer* renderer, Range<int> range, int section, SDL_RendererFlip flip);

	operator SDL_Texture*();

	~Texture();
};

