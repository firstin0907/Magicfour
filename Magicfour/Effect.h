#pragma once
#include "AutoDeleteFieldObject.h"
class Effect :
	public AutoDeleteFieldObject
{
protected:
	char	alpha_;		// 이미지 파일 이름의 첫 글자(알파벳)
	int		id_;		// 이미지 파일 이름의 알파벳 다음 숫자
	
	std::shared_ptr<Texture>	texture_;	// alpha == 0일 때, 그릴 텍스쳐

	Uint32				frame_time_;		// 프레임 하나가 몇 ms동안 보여질 건지
	SDL_RendererFlip	flip_;				// flip 옵션


public:
	// parent_object_manager: 이 객체가 속해있는 ObjectManager 객체
	// alpha, id			: 어떤 이미지를 그릴 것인지
	// create_time			: 이 객체가 생성될 시간
	// frame_time			: 한 프레임에서 다음 프레임으로 넘어가기까지 시간
	// position, flip		: 그려질 위치와 방향
	// v_per_s				: 초당 몇 픽셀을 움직이게 할 것인지
	// life_time			: 이 객체가 소멸하지 않고 유지될 시간
	Effect(
		ObjectManager& parent_object_manager, char alpha, int id,
		Uint32 create_time, Uint32 frame_time, Point<double> position,
		SDL_RendererFlip flip, Vector2d<double> v_per_s = { 0, 0 },
		Uint32 life_time = ETERNAL);


	virtual bool	update(const TimeManager& tm);
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	~Effect();
};

