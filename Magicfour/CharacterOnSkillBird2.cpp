#include "CharacterOnSkillBird2.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxBead.h"


CharacterOnSkillBird2::CharacterOnSkillBird2(ObjectManager& parent_object_manager)
	: Character(parent_object_manager, 0)
{
}

void CharacterOnSkillBird2::set_up(const Character *instance)
{
	Character::set_up(instance);
	set_a_y(0, skill_use_time_);
	set_v_y(-400 * 10, skill_use_time_);

	cnt_ = 0;
	next_create_ = skill_use_time_ + 200;
}

void CharacterOnSkillBird2::set_state(State state, const Uint32 modify_time)
{
	if (state != LAND) FieldObject::set_state(state, modify_time);

	switch (state)
	{
	case ON_SKILL1:
		set_v_y(0, modify_time);
		set_a_y(0, modify_time);
		break;

	case ON_SKILL2:
		set_a_y(GRAVITY, modify_time);
		break;
	}
}

bool CharacterOnSkillBird2::update(const TimeManager& tm)
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
			// ��ų ��� ���� �ϴ÷� �ö󰡰� �ִ� ����
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + 100, tm);
			break;

		case ON_SKILL1:
			// �ϴ÷� �ö� ������ �� ������ ����
			run = set_state_if_time(ON_SKILL2, skill_use_time_ + 1600, tm);
			break;
			
		case ON_SKILL2:
			// ������ ������� ���ƿ��� ������ �������� �ְų� ���� ������ ��
			return set_state_if_time(GRACE_AFTER_LONG_SKILL, skill_use_time_ + 2000, tm) == false;
		}
	} while (run);

	while (next_create_ <= tm.get_curr() && cnt_ <= 20)
	{
		// angle�� [0, 3.141 = �뷫 pi] ���� ���� ��
		double angle = RandomGenerator::rand(0, 3141) * 0.001;

		object_manager_.insert(std::make_shared<DamageBoxBead>(object_manager_, next_create_,
			position_, Vector2d<double>(std::cos(angle), std::sin(angle)) * 1400, 2),
			ObjectManager::ALLIES);

		++cnt_;
		next_create_ += 100;
	}

	return true;
}

void CharacterOnSkillBird2::draw(Renderer& renderer, const TimeManager & tm)
{
	switch (state_)
	{
	case ON_SKILL:
	case ON_SKILL2:
		renderer.enroll_texture('C', 4, position_, 0, get_flip());
		break;

	case ON_SKILL1:
		renderer.enroll_texture('O', 4, position_, 0, get_flip());
		break;
	}
}


CharacterOnSkillBird2::~CharacterOnSkillBird2()
{
}
