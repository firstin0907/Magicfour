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

	// modify_time �������� state�� �����ϰ�, ���� �� �ؾ��� �۾��� �����Ѵ�.
		// �Ű����� state�� LAND�� ���� state�� �ٲ��� �� ��
		// �Ű����� state�� LAND�� ���� v_, a_ �� ���� ����(�Լ� ������� �� ��) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// ��� ��ü�� �浹���� �� � �۾��� ���� ����
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class�� ��ü�� �׸� ������ ������.
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
		// ���鿡 ������ �� �̻��� �浹 ������ ���� �ʴ´�.
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
