#include "CharacterOnSkillDuck2.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxThorn.h"

constexpr Uint32		HIGHIST_TIME = 200;
constexpr Uint32		GO_DOWN_TIME = 1500;

constexpr double		GO_UP_V = -6'000;
constexpr double		GO_DOWN_V = 12'000;

CharacterOnSkillDuck2::CharacterOnSkillDuck2(ObjectManager& parent_object_manager)
	: Character(parent_object_manager, 0)
{
}

void CharacterOnSkillDuck2::set_up(const Character* instance)
{
	Character::set_up(instance);

	add_v_y(GO_UP_V, skill_use_time_);
	set_a({ 0, 0 }, skill_use_time_);

	cnt_ = 0;

	// ������ �ö� �������� ���� ��ô ����
	next_drop_ = skill_use_time_ + HIGHIST_TIME;
}


void CharacterOnSkillDuck2::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);
	switch (state)
	{
	case ON_SKILL1:
		set_v_y(0, modify_time);
		break;

	case ON_SKILL2:
		set_v_y(GO_DOWN_V, modify_time);
		break;

	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;
		
	case ON_SKILL3:
		object_manager_.insert(std::make_shared<DamageBox>(object_manager_, modify_time,
			Point<int>(static_cast<int>(position_.x), GROUND_Y - 1),
			Range<int>({ -120, -30 }, { 120, 1 }), 5,
			Vector2d<double>(0, 0), 500, 1 << 30), ObjectManager::ALLIES);
	}

}

bool CharacterOnSkillDuck2::update(const TimeManager& tm)
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
			// ��ų ��� ���� ~ ������ �ö󰡱� ����
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + HIGHIST_TIME, tm);
			break;

		case ON_SKILL1:
			// ������ �ö� ����
			run = set_state_if_time(ON_SKILL2, skill_use_time_ + GO_DOWN_TIME, tm);
			break;

		case ON_SKILL2:
			// �������� ������ �������� �ִ� ����
			run = set_state_if_time(ON_SKILL3, skill_use_time_ + GO_DOWN_TIME + 105, tm);
			break;

		case ON_SKILL3:
			// ������ ������ ������ ����
			return set_state_if_time(GRACE_AFTER_LONG_SKILL, skill_use_time_ + 2'000, tm) == false;
		}
	} while (run);

	// 20���� ���ø� ��ô
	while (cnt_ <= 20 && tm.get_curr() > next_drop_)
	{
		++cnt_;
		Point<double> drop_position = { static_cast<double>(RandomGenerator::rand(0, GAME_W)), -10.0 };

		object_manager_.insert(std::make_shared<DamageBoxThorn<0, 2500> >(
			object_manager_, next_drop_, drop_position, 2), ObjectManager::ALLIES);

		next_drop_ += 30;
	}

	return true;
}

void CharacterOnSkillDuck2::draw(Renderer& renderer, const TimeManager& tm)
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

CharacterOnSkillDuck2::~CharacterOnSkillDuck2()
{
}
