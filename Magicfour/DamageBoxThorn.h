#pragma once
#include "DamageBox.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

template <int X_SPEED, int Y_SPEED> 
class DamageBoxThorn :
	public DamageBox
{
public:
	DamageBoxThorn(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, int damage);

	// modify_time 기준으로 state를 변경하고, 변경 시 해야할 작업을 수행한다.
		// 매개변수 state가 LAND일 때는 state값 바꾸지 말 것
		// 매개변수 state가 LAND일 떄는 v_, a_ 값 직접 변경(함수 사용하지 말 것) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// 상대 객체와 충돌했을 때 어떤 작업을 해줄 건지
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBoxThorn();
};

template <int X_SPEED, int Y_SPEED>
DamageBoxThorn<X_SPEED, Y_SPEED>::DamageBoxThorn(ObjectManager& parent_object_manager, Uint32 create_time,
	Point<double> position, int damage)
	: DamageBox(parent_object_manager, create_time, position,
		Range<int>({ 0, 0 }, 8), damage, { X_SPEED, Y_SPEED }, 300, 1, 0.2)
{

}

template<int X_SPEED, int Y_SPEED>
inline void DamageBoxThorn<X_SPEED, Y_SPEED>::set_state(State state, const Uint32 modify_time)
{
	if (state_ != DIE && state == LAND)
	{
		// 지면에 닿으면 더 이상의 충돌 판정을 하지 않는다.
		v_ = a_ = { 0, 0 };
		state_ = DIE;
	}
}

template <int X_SPEED, int Y_SPEED>
bool DamageBoxThorn<X_SPEED, Y_SPEED>::update(const TimeManager& tm)
{
	if (DamageBox::update(tm) == false) return false;
	return true;
}


template<int X_SPEED, int Y_SPEED>
bool DamageBoxThorn<X_SPEED, Y_SPEED>::on_hit(
	std::weak_ptr<FieldObject> opponent, const TimeManager& tm,
	Renderer& renderer, SoundManager& sm)
{

	if (DamageBox::on_hit(opponent, tm, renderer, sm))
	{
		sm.play_sfx(4);
		return true;
	}
	else return false;
}


template<int X_SPEED, int Y_SPEED>
inline void DamageBoxThorn<X_SPEED, Y_SPEED>::draw(Renderer& renderer, const TimeManager& tm)
{
	if constexpr (X_SPEED == 0)
	{
		renderer.enroll_texture('O', 0, position_,
			1 * (position_.y >= GROUND_Y - 8), SDL_FLIP_NONE);
	}
	else if constexpr (X_SPEED == Y_SPEED)
	{
		renderer.enroll_texture('O', 2, position_,
			1 * (position_.y >= GROUND_Y - 8), SDL_FLIP_NONE);
	}
	else if constexpr (X_SPEED == -Y_SPEED)
	{
		renderer.enroll_texture('O', 2, position_,
			1 * (position_.y >= GROUND_Y - 8), SDL_FLIP_HORIZONTAL);
	}
	else if constexpr (X_SPEED > 0)
	{
		renderer.enroll_texture('O', 1, position_,
			1 * (position_.y >= GROUND_Y - 8), SDL_FLIP_NONE);
	}
	else {
		renderer.enroll_texture('O', 1, position_,
			1 * (position_.y >= GROUND_Y - 8), SDL_FLIP_HORIZONTAL);
	}
}

template <int X_SPEED, int Y_SPEED>
DamageBoxThorn<X_SPEED, Y_SPEED>::~DamageBoxThorn()
{
}
