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
	// ������ ���߰ų�, ���� ���� Ƚ���� 0�� �� false return
	return AutoDeleteFieldObject::update(tm) && remain_penetration_ > 0;
}

bool DamageBox::on_hit(std::weak_ptr<FieldObject> opponent, const TimeManager& tm,
	Renderer& renderer, SoundManager& sm)
{
	// ������ �ڽ��� ������ �� �ִ� Ƚ���� ���ų�, �浹�� �� �� ������ ���� ���� �ǳʶڴ�.
	if (remain_penetration_ <= 0 || object_hit[opponent] + 500 >= tm.get_curr())
	{
		return false;
	}

	object_hit[opponent] = tm.get_curr();

	Monster* monster_opp = dynamic_cast<Monster*>(opponent.lock().get());
	if (monster_opp)
	{
		// �浹�� FieldObject�� Monster�� ����� ó��

		if (monster_opp->damage(damage_, tm.get_curr(), renderer))
		{
			// ���Ϳ��� �������� �ְ�, ���� ������ state�� HIT_KNOCK_BACK���� �ٲ�.
			// (���� �׾��ٸ� Monster::damage���� state�� DIE�� �ٲ�)
			monster_opp->set_v(v_ * 1'000 * power_, tm.get_curr());
			monster_opp->set_state(FieldObject::HIT_KNOCK_BACK, tm.get_curr());
		}

		// Monster�� �̵� ������ DamageBox�� �̵� ����� �ݴ�� �����Ѵ�.
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
