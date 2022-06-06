#include "CharacterOnAttack.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxNormalAttack.h"


CharacterOnAttack::CharacterOnAttack(ObjectManager& parent_object_manager)
	: Character(parent_object_manager, 0)
{
}

void CharacterOnAttack::set_up(const Character * instance)
{
	Character::set_up(instance);
}

void CharacterOnAttack::set_state(State state, const Uint32 modify_time)
{
	if (state != LAND) FieldObject::set_state(state, modify_time);
	switch (state)
	{
	case ON_SKILL1:
		object_manager_.insert(std::make_shared<DamageBoxNormalAttack>(object_manager_,
			modify_time, position_.x, v_ * 1000, direction_), ObjectManager::ALLIES);
		break;

	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;
	}
}

bool CharacterOnAttack::update(const TimeManager& tm)
{
	// 캐릭터가 화면 밖으로 나가지 않도록 위치 조정
	if (position_.x < 0) position_.x = 0;
	else if (position_.x > GAME_W) position_.x = GAME_W;

	bool run = false;
	do
	{
		switch (state_)
		{
		case ON_SKILL:
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + 100, tm);

		case ON_SKILL1:
			return set_state_if_time(GRACE_AFTER_SKILL, skill_use_time_ + 420, tm) == false;
		}
	} while (run);

	return true;
}

void CharacterOnAttack::draw(Renderer& renderer, const TimeManager& tm)
{
	renderer.enroll_texture('C', 1, position_, (tm - skill_use_time_) / 30, get_flip());
}



CharacterOnAttack::~CharacterOnAttack()
{
}
