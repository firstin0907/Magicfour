#pragma once
#include "AutoDeleteFieldObject.h"
class Effect :
	public AutoDeleteFieldObject
{
protected:
	char	alpha_;		// �̹��� ���� �̸��� ù ����(���ĺ�)
	int		id_;		// �̹��� ���� �̸��� ���ĺ� ���� ����
	
	std::shared_ptr<Texture>	texture_;	// alpha == 0�� ��, �׸� �ؽ���

	Uint32				frame_time_;		// ������ �ϳ��� �� ms���� ������ ����
	SDL_RendererFlip	flip_;				// flip �ɼ�


public:
	// parent_object_manager: �� ��ü�� �����ִ� ObjectManager ��ü
	// alpha, id			: � �̹����� �׸� ������
	// create_time			: �� ��ü�� ������ �ð�
	// frame_time			: �� �����ӿ��� ���� ���������� �Ѿ����� �ð�
	// position, flip		: �׷��� ��ġ�� ����
	// v_per_s				: �ʴ� �� �ȼ��� �����̰� �� ������
	// life_time			: �� ��ü�� �Ҹ����� �ʰ� ������ �ð�
	Effect(
		ObjectManager& parent_object_manager, char alpha, int id,
		Uint32 create_time, Uint32 frame_time, Point<double> position,
		SDL_RendererFlip flip, Vector2d<double> v_per_s = { 0, 0 },
		Uint32 life_time = ETERNAL);


	virtual bool	update(const TimeManager& tm);
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	~Effect();
};

