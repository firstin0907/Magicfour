#include "DamageBoxBead.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

DamageBoxBead::DamageBoxBead(ObjectManager& parent_object_manager, Uint32 create_time,
	Point<double> position, Vector2d<double> v_per_s, int damage)
	: DamageBox(parent_object_manager, create_time, position, Range<int>({0, 0}, 14), damage, v_per_s, 700, 2, 0.3)
{

}

bool DamageBoxBead::update(const TimeManager & tm)
{
	return DamageBox::update(tm);
}

bool DamageBoxBead::on_hit(std::weak_ptr<FieldObject> opponent, const TimeManager& tm,
	Renderer& renderer, SoundManager& sm)
{
	if (DamageBox::on_hit(opponent, tm, renderer, sm))
	{
		if (remain_penetration_ == 1)
		{
			// 충돌 시 속도를 70%으로 만든다.
			set_v(v_ * 700, tm.get_curr());
		}
		return true;
	}
	return false;
}

void DamageBoxBead::draw(Renderer& renderer, const TimeManager& tm)
{
	renderer.enroll_texture('O', 5, position_, 0, SDL_FLIP_NONE);
}

DamageBoxBead::~DamageBoxBead()
{
}
