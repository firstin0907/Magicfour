#include "MonsterOctopus.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "Character.h"

MonsterOctopus::MonsterOctopus(ObjectManager& parent_object_manager, Uint32 create_time)
	: Monster(parent_object_manager, create_time,
		Point<int>(-43, GROUND_Y),
		Range<int>({ -20, -61 }, { 30, 0 }),
		Vector2d<double>(150.0, 0), max_hp_)
{
	next_change_ = create_time + RandomGenerator::rand(3400, 6000);
}

void MonsterOctopus::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);
	switch (state)
	{
	case NORMAL:
		set_v({ 150.0 * direction_weight(), 0.0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);
		next_change_ = modify_time + RandomGenerator::rand(1400, 6000);
		break;

	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;

	case HIT_KNOCK_BACK:
		set_a(v_ * 1'000 * (-4), modify_time);
		add_a_y(GRAVITY, modify_time);
		break;

	case HIT:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);
		break;

	case TURN:
		if (direction_ == LEFT_HEAD) change_direction(RIGHT_HEAD);
		else change_direction(LEFT_HEAD);

		set_state(NORMAL, modify_time);
		break;

	case DIE:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);

		// ���� óġ �� 30%�� Ȯ���� ĳ���� ��ų �߰�
		if (RandomGenerator::rand(1, 100) <= 30) object_manager_.curr_character_->add_skill(1, modify_time);
		break;
	}
}

bool MonsterOctopus::update(const TimeManager& tm)
{
	if (Monster::update(tm) == false)
	{
		return false;
	}

	bool run = false;
	do
	{
		switch (state_)
		{
		case NORMAL:
			// x�����θ� �̵��ϰ� �ִ�, �Ϲ� ����
			run = set_state_if_time(TURN, next_change_, tm);
			break;

		case HIT_KNOCK_BACK:
			// �ǰݴ��� ���Ķ� �и��� �ִ� ����
			run = set_state_if_time(HIT, state_start_time_ + 500 / 2, tm);
			break;

		case HIT:
			// �ǰݴ��� �� �� �и��� �� ��
			run = set_state_if_time(NORMAL, state_start_time_ + 500 / 2, tm);
			break;

		case DIE:
			// ��� ���� -> �Ҹ���� 0.8s �ҿ�
			run = set_state_if_time(ERASED, state_start_time_ + 800, tm);
			break;
			
		case ERASED:
			return false;
		}

	} while (run);

	return true;
}

void MonsterOctopus::draw(Renderer & renderer, const TimeManager & tm)
{
	switch (state_)
	{
	case NORMAL:
		renderer.enroll_texture('M', 6, position_, keep_state_time(tm) / 100, get_flip());
		break;

	case HIT:
	case HIT_KNOCK_BACK:
		renderer.enroll_texture('M', 7, position_, 0, get_flip());
		break;

	case DIE:
		renderer.enroll_texture('M', 8, position_, std::min(5U, keep_state_time(tm) / 100), get_flip());
		break;
	}
}



MonsterOctopus::~MonsterOctopus()
{
}
