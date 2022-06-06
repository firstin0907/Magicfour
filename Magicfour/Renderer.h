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

	// renderer�� ��µ� ������ ��� �����.
	void clear();

	// renderer�� ������ �����Ѵ�.
	void set_color(Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	// �簢�� �Ǵ� ���� �׸���.
	void draw_range(const Range<int>& range);
	
	// ��ĥ�� �簢���� �׸���.
	void draw_rect(const Range<int>& range);

	// ����� �ؽ��߸� return
	Texture& get_texture(char alpha, int id);

	// �ؽ��ĸ� ����Ѵ�.
	void enroll_texture(std::weak_ptr<Texture> texture, Point<int> point,
		int section, SDL_RendererFlip flip);
	// �ؽ��ĸ� ����Ѵ�.
	void enroll_texture(char alpha, int id, Point<int> point,
		int section, SDL_RendererFlip flip);
	// �ؽ��ĸ� ����Ѵ�.
	void enroll_texture(char alpha, int id, Range<int> range,
		int section, SDL_RendererFlip flip);
	// �ؽ��ĸ� ����Ѵ�.
	void enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect);
	// �ؽ��ĸ� ����Ѵ�.
	void enroll_texture(char alpha, int id, Range<int> src_rect, Range<int> dst_rect,
		int angle, Point<int> center, SDL_RendererFlip flip);
	
	// �ؽ�Ʈ�� Texture�� ���� return �Ѵ�.
	std::shared_ptr<Texture> make_text(std::string message, int font_num, SDL_Color color);

	// renderer�� ��µ� ������ ȭ�鿡 ����Ѵ�.
	void present();

	operator SDL_Renderer*&();

	~Renderer();
};
