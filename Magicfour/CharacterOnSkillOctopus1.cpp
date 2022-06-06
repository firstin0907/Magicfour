#include "CharacterOnSkillOctopus1.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxLeg.h"

CharacterOnSkillOctopus1::CharacterOnSkillOctopus1(ObjectManager& parent_object_manage)
	: Character(parent_object_manage, 0)
{
}

void CharacterOnSkillOctopus1::set_up(const Character* instance)
{
	Character::set_up(instance);
	
}

void CharacterOnSkillOctopus1::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);

	switch (state)
	{
	case ON_SKILL1:
		// ������ �ڽ� ����
		const double target_x = std::min(std::max(position_.x + 200 * direction_weight(), 50.0), GAME_W - 50.0);

		object_manager_.insert(std::make_shared<DamageBoxLeg>(
			object_manager_, modify_time, target_x), ObjectManager::ALLIES);
	}
}

bool CharacterOnSkillOctopus1::update(const TimeManager& tm)
{
	// ĳ���Ͱ� ȭ�� ������ ������ �ʵ��� ��ġ ����
	if (position_.x < 0) position_.x = 0;
	else if (position_.x > GAME_W) position_.x = GAME_W;

	bool run = false;
	do
	{
		switch (state_)
		{

		case ON_SKILL:
			// ��ų ��� ���� �ٷ� state_ ��ȯ
			run = set_state_if_time(ON_SKILL1, state_start_time_, tm);
			break;

		case ON_SKILL1:
			return set_state_if_time(GRACE_AFTER_SKILL, skill_use_time_ + 600, tm) == false;
			break;
		}

	} while (run);
	

	return true;
}

void CharacterOnSkillOctopus1::draw(Renderer& renderer, const TimeManager& tm)
{
	// 1. ��ų ��� �� 0.42�ʰ� ������ �ʾҴٸ� ��ų ���
	if (tm - skill_use_time_ < 420)
	{
		renderer.enroll_texture('C', 1, position_, (tm - skill_use_time_) / 30, get_flip());
	}
	//     �ƴϸ� �Ϲ� �������!
	else
	{
		renderer.enroll_texture('C', 4, position_, (tm - skill_use_time_) / 30, get_flip());
	}
	draw_ui(renderer, tm);
}

CharacterOnSkillOctopus1::~CharacterOnSkillOctopus1()
{
}
