#pragma once
#include "header.h"

struct Texture
{
	SDL_Texture*	texture;
	Vector2d<int>	size;

	Point<int>		center;
	int				section_num;


public:
	// 이미지 파일을 이용한 texture 만들기!
	Texture(std::string img_file_name, std::string data_file_name,
		SDL_Renderer* renderer);

	// font를 이용한 texture 만들기!
	Texture(std::string message, SDL_Renderer* renderer, TTF_Font* font,
		SDL_Color color = { 255, 255, 255, 255 });

	// move 생성자
	Texture(Texture&& t) noexcept;

	// renderer에 texture의 중심(center)가 point에 가게끔 그린다. 출력되는 크기는 그대로다.
	void draw(SDL_Renderer* renderer, Point<int> point, int section, SDL_RendererFlip flip);

	// renderer의 range 부분에 그린다. 따라서 출력되는 크기가 달라질 수 있다.
	void draw(SDL_Renderer* renderer, Range<int> range, int section, SDL_RendererFlip flip);

	operator SDL_Texture*();

	~Texture();
};

