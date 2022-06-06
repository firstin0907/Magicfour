#include "DamageBox.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "FieldObject.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "Monster.h"

DamageBox::DamageBox(ObjectManager& parent_object_manager, Uint32 create_time,
	Point<double> position, Range<int> range, int damage, Vector2d<double> v_per_s,
	Uint32 life_time, int penetration_number, double power)
	: AutoDeleteFieldObject(parent_object_manager, create_time, life_time, position, range, v_per_s),
	damage_(damage), power_(power), remain_penetration_(penetration_number)
{
}



bool DamageBox::update(const TimeManager& tm)
{
	// 수명이 다했거나, 남은 관통 횟수가 0일 때 false return
	return AutoDeleteFieldObject::update(tm) && remain_penetration_ > 0;
}

bool DamageBox::on_hit(std::weak_ptr<FieldObject> opponent, const TimeManager& tm,
	Renderer& renderer, SoundManager& sm)
{
	// 데미지 박스가 관통할 수 있는 횟수가 없거나, 충돌한 지 얼마 지나지 않은 경우는 건너뛴다.
	if (remain_penetration_ <= 0 || object_hit[opponent] + 500 >= tm.get_curr())
	{
		return false;
	}

	object_hit[opponent] = tm.get_curr();

	Monster* monster_opp = dynamic_cast<Monster*>(opponent.lock().get());
	if (monster_opp)
	{
		// 충돌한 FieldObject가 Monster인 경우의 처리

		if (monster_opp->damage(damage_, tm.get_curr(), renderer))
		{
			// 몬스터에게 데미지를 주고, 죽지 않으면 state를 HIT_KNOCK_BACK으로 바꿈.
			// (만약 죽었다면 Monster::damage에서 state를 DIE로 바꿈)
			monster_opp->set_v(v_ * 1'000 * power_, tm.get_curr());
			monster_opp->set_state(FieldObject::HIT_KNOCK_BACK, tm.get_curr());
		}

		// Monster의 이동 방향을 DamageBox의 이동 방향과 반대로 설정한다.
		if (v_.x > 0) monster_opp->change_direction(FieldObject::LEFT_HEAD);
		else if (v_.x < 0) monster_opp->change_direction(FieldObject::RIGHT_HEAD);

		--remain_penetration_;
	}
	return true;
}

void DamageBox::draw(Renderer& renderer, const TimeManager& tm)
{

}


DamageBox::~DamageBox()
{

}
