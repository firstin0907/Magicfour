#include "CharacterOnSkillBird1.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxBead.h"

CharacterOnSkillBird1::CharacterOnSkillBird1(ObjectManager& parent_object_manager)
	: Character(parent_object_manager, 0)
{
}

void CharacterOnSkillBird1::set_up(const Character * instance)
{
	Character::set_up(instance);
	x_speed = 1'400 * direction_weight();
}

void CharacterOnSkillBird1::set_state(State state, const Uint32 modify_time)
{
	// state ���ڰ� LAND�� ���� state_�� ���� �ٲ����� �ʴ´�.
	if (state != LAND) FieldObject::set_state(state, modify_time);

	switch (state)
	{
	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;

	case ON_SKILL1:
		object_manager_.insert(std::make_shared<DamageBoxBead>(object_manager_, modify_time,
			position_ - UNIT_VECTOR2D_Y * 68, Vector2d<double>(x_speed, 50), 2), ObjectManager::ALLIES);
		break;
	case ON_SKILL2:
		object_manager_.insert(std::make_shared<DamageBoxBead>(object_manager_, modify_time,
			position_ - UNIT_VECTOR2D_Y * 68, Vector2d<double>(x_speed, -100), 2), ObjectManager::ALLIES);
		break;
	case ON_SKILL3:
		object_manager_.insert(std::make_shared<DamageBoxBead>(object_manager_, modify_time,
			position_ - UNIT_VECTOR2D_Y * 68, Vector2d<double>(x_speed, -250), 2), ObjectManager::ALLIES);
		break;
	}
}

bool CharacterOnSkillBird1::update(const TimeManager & tm)
{
	// ĳ���Ͱ� ȭ�� ������ ������ �ʵ��� ��ġ ����
	if (position_.x < 0) position_.x = 0;
	else if (position_.x > GAME_W) position_.x = GAME_W;


	// state�� ���� ó��. state�� �ٲ�� ó���� �ݺ��Ѵ�.
	bool run = false;
	do
	{
		run = false;
		switch (state_)
		{
		case ON_SKILL:
			// ��ų ��� ����
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + 140, tm);
			break;

		case ON_SKILL1:
			// ù ��° DamageBox ���� ��
			run = set_state_if_time(ON_SKILL2, skill_use_time_ + 280, tm);
			break;

		case ON_SKILL2:
			// �� ��° DamageBox ���� ��
			run = set_state_if_time(ON_SKILL3, skill_use_time_ + 420, tm);
			break;

		case ON_SKILL3:
			// �� ��°(������) DamageBox ���� ��
			return set_state_if_time(GRACE_AFTER_SKILL, skill_use_time_ + 500, tm) == false;
		}
	} while (run);

	return true;
}

void CharacterOnSkillBird1::draw(Renderer & renderer, const TimeManager & tm)
{
	if (tm - skill_use_time_ < 420)
	{
		// 0.140�ʿ� ���� ��� 1ȸ��
		renderer.enroll_texture('C', 1, position_, (tm - skill_use_time_) / 10, get_flip());
	}
	else
	{
		// �� ��° ������ �ڽ����� �����ϰ� �� �� �Ϲ� ���
		renderer.enroll_texture('C', 4, position_, (tm - skill_use_time_) / 30, get_flip());
	}
}


CharacterOnSkillBird1::~CharacterOnSkillBird1()
{
}
