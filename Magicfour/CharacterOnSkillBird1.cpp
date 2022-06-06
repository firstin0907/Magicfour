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
	// state 인자가 LAND일 때는 state_의 값을 바꿔주지 않는다.
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
	// 캐릭터가 화면 밖으로 나가지 않도록 위치 조정
	if (position_.x < 0) position_.x = 0;
	else if (position_.x > GAME_W) position_.x = GAME_W;


	// state에 따른 처리. state가 바뀌면 처리를 반복한다.
	bool run = false;
	do
	{
		run = false;
		switch (state_)
		{
		case ON_SKILL:
			// 스킬 사용 직후
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + 140, tm);
			break;

		case ON_SKILL1:
			// 첫 번째 DamageBox 생성 후
			run = set_state_if_time(ON_SKILL2, skill_use_time_ + 280, tm);
			break;

		case ON_SKILL2:
			// 두 번째 DamageBox 생성 후
			run = set_state_if_time(ON_SKILL3, skill_use_time_ + 420, tm);
			break;

		case ON_SKILL3:
			// 세 번째(마지막) DamageBox 생성 후
			return set_state_if_time(GRACE_AFTER_SKILL, skill_use_time_ + 500, tm) == false;
		}
	} while (run);

	return true;
}

void CharacterOnSkillBird1::draw(Renderer & renderer, const TimeManager & tm)
{
	if (tm - skill_use_time_ < 420)
	{
		// 0.140초에 공격 모션 1회전
		renderer.enroll_texture('C', 1, position_, (tm - skill_use_time_) / 10, get_flip());
	}
	else
	{
		// 세 번째 데미지 박스까지 생성하고 그 후 일반 모션
		renderer.enroll_texture('C', 4, position_, (tm - skill_use_time_) / 30, get_flip());
	}
}


CharacterOnSkillBird1::~CharacterOnSkillBird1()
{
}
