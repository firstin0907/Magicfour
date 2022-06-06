#include "MonsterBird.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "FieldObject.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "Character.h"
#include "Effect.h"

constexpr Uint32 REPOSITON_TIME = 500;

MonsterBird::MonsterBird(ObjectManager& parent_object_manager, Uint32 create_time)
	: Monster(parent_object_manager, create_time,
		Point<int>(-30, GROUND_Y / 2),
		Range<int>({ -70, -38 }, { 70, 13 }),
		Vector2d<double>(260.0, 0.0), max_hp_)
{
	y_destination = 260;
	next_move_ = create_time + RandomGenerator::rand(2500, 4000);;
}

void MonsterBird::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);
	double rep_t = REPOSITON_TIME / 1'000.0;
	switch (state)
	{
	case NORMAL:
		state_ = FLY;

	case FLY:
		y_destination = 150 * RandomGenerator::rand(1, 3);
		next_move_ = modify_time + RandomGenerator::rand(2500, 4000);
		set_v({ 260.0 * direction_weight(), 0 }, modify_time);
		set_a({ 0, (y_destination - position_.y) / rep_t / rep_t }, modify_time);
		break;
	case LAND:
		v_.y = a_.y = 0;
		break;

	case DECEL:
		set_a({ 0, -(y_destination - position_.y) * 2 / rep_t / rep_t }, modify_time);
		break;

	case GLIDING:
		set_v({ 260.0 * direction_weight(), 0 }, modify_time);
		set_a({ 0, 0 }, modify_time);
		break;		

	case HIT_KNOCK_BACK:
		set_a(v_ * 1'000 * (-4), modify_time);
		break;

	case HIT:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, 0 }, modify_time);
		break;

	case DIE:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);

		// 몬스터 처치 시 27%의 확률로 캐릭터 스킬 추가
		if (RandomGenerator::rand(1, 100) <= 27) object_manager_.curr_character_->add_skill(2, modify_time);
	}
}

bool MonsterBird::update(const TimeManager& tm)
{
	if (Monster::update(tm) == false) return false;

	bool run = true;
	do
	{
		run = false;
		switch (state_)
		{
		case NORMAL:
		case FLY:
			// 있어야 하는 위치(y_destination)으로 돌아가려고 하는 상태
			run = set_state_if_time(DECEL, state_start_time_ + REPOSITON_TIME, tm);
			break;

		case DECEL:
			// 있어야 하는 위치에서 멈추기 위해 속도 줄이고 있는 상태
			run = set_state_if_time(GLIDING, state_start_time_ + REPOSITON_TIME, tm);
			break;

		case GLIDING:
			// 있어야하는 위치에 있어서 y축 변화가 없는 상태
			run = set_state_if_time(FLY, next_move_, tm);
			break;

		case HIT_KNOCK_BACK:
			// 피격당한 직후라서 밀리고 있는 상태
			run = set_state_if_time(HIT, state_start_time_ + 500 / 2, tm);
			break;

		case HIT:
			// 피격당한 후 다 밀리고 난 뒤
			run = set_state_if_time(FLY, state_start_time_ + 500 / 2, tm);
			break;

		case DIE:
			// 사망 상태 -> 소멸까지 0.7s 소요
			run = set_state_if_time(ERASED, state_start_time_ + 700, tm);
			break;

		case ERASED:
			// 객체가 지워저야 하는 상태
			return false;
		}
	} while (run);

	return true;
}

void MonsterBird::draw(Renderer& renderer, const TimeManager& tm)
{
	switch (state_)
	{
	case NORMAL:
	case FLY:
		renderer.enroll_texture('M', 3, position_, keep_state_time(tm) / 50, get_flip());
		break;

	case DECEL:
	case GLIDING:
		renderer.enroll_texture('M', 3, position_, 1, get_flip());
		break;

	case HIT:
	case HIT_KNOCK_BACK:
		renderer.enroll_texture('M', 4, position_, 1, get_flip());
		break;

	case DIE:
		renderer.enroll_texture('M', 5, position_, std::min(2U, keep_state_time(tm) / 200), get_flip());
		break;
	}
}

MonsterBird::~MonsterBird()
{
}
