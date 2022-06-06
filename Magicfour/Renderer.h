#pragma once
#include "header.h"
#include "utility.h"

#include "Texture.h"

class Renderer
{
	SDL_Renderer*			renderer_;
	Vector2d<Uint32>		logical_size_;

	std::vector<Texture>	texture_[30];
	std::vector<TTF_Font*>	font_;

public:
	Renderer(SDL_Window* window, Vector2d<Uint32> logical_size);

	// renderer에 출력된 내용을 모두 지운다.
	void clear();

	// renderer의 색깔을 설정한다.
	void set_color(Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	// 사각형 또는 원을 그린다.
	void draw_range(const Range<int>& range);
	
	// 색칠된 사각형을 그린다.
	void draw_rect(const Range<int>& range);

	// 저장된 텍스텨를 return
	Texture& get_texture(char alpha, int id);

	// 텍스쳐를 출력한다.
	void enroll_texture(std::weak_ptr<Texture> texture, Point<int> point,
		int section, SDL_RendererFlip flip);
	// 텍스쳐를 출력한다.
	void enroll_texture(char alpha, int id, Point<int> point,
		int section, SDL_RendererFlip flip);
	// 텍스쳐를 출력한다.
	void enroll_texture(char alpha, int id, Range<int> range,
		int section, SDL_RendererFlip flip);
	// 텍스쳐를 출력한다.
	void enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect);
	// 텍스쳐를 출력한다.
	void enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect,
		int angle, Point<int> center, SDL_RendererFlip flip);
	
	// 텍스트를 Texture로 만들어서 return 한다.
	std::shared_ptr<Texture> make_text(std::string message, int font_num, SDL_Color color);

	// renderer에 출력된 내용을 화면에 출력한다.
	void present();

	operator SDL_Renderer*&();

	~Renderer();
};
