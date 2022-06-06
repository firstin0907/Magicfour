#include "MonsterDuck.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "Character.h"

constexpr double	JUMP_V = -1'300;
constexpr double	X_SPEED = 200;

MonsterDuck::MonsterDuck(ObjectManager &parent_object_manager, Uint32 create_time)
	: Monster(parent_object_manager, create_time,
		Point<int>(-30, GROUND_Y),
		Range<int>({ -20, -80 }, { 30, 0 }),
		Vector2d<double>(X_SPEED, 0.0), max_hp_)
{
	next_jump_ = create_time + RandomGenerator::rand(2400, 3400);
}

void MonsterDuck::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);
	switch (state)
	{
	case NORMAL:
		set_v({ X_SPEED * direction_weight(), 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);
		next_jump_ = modify_time + RandomGenerator::rand(1400, 2400);
		break;

	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		if (state_ == JUMP)
		{
			v_.x = X_SPEED * 0.001 * direction_weight();
			FieldObject::set_state(NORMAL, modify_time);
		}
		next_jump_ = modify_time + RandomGenerator::rand(1400, 2400);
		break;

	case JUMP_READY:
		set_v({ 0, 0 }, modify_time);
		break;

	case JUMP:
		set_v({ X_SPEED * 2 * direction_weight(), JUMP_V }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);
		next_jump_ = modify_time + RandomGenerator::rand(1400, 2400);
		break;

	case HIT_KNOCK_BACK:
		set_a(v_ * 1'000 * (-4), modify_time);
		add_a_y(GRAVITY, modify_time);
		break;

	case HIT:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, 0 }, modify_time);
		break;

	case DIE:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);

		// 몬스터 처치 시 35%의 확률로 캐릭터 스킬 추가
		if(RandomGenerator::rand(1, 100) <= 35) object_manager_.curr_character_->add_skill(0, modify_time);
		break;
	}
}

bool MonsterDuck::update(const TimeManager& tm)
{
	if (Monster::update(tm) == false) return false;

	bool run = false;
	do
	{
		run = false;
		switch (state_)
		{
		case NORMAL:
			// x축으로만 이동하고 있는, 일반 상태
			run = set_state_if_time(JUMP_READY, next_jump_ - 200, tm);
			break;

		case JUMP_READY:
			// 점프하려고 잠깐 멈칫하고 있는 상태
			run = set_state_if_time(JUMP, next_jump_, tm);
			break;

		case JUMP:
			// 점프하고 있는 상태
			break;

		case HIT_KNOCK_BACK:
			// 피격당한 직후라서 밀리고 있는 상태
			run = set_state_if_time(HIT, state_start_time_ + 500 / 2, tm);
			break;

		case HIT:
			// 피격당한 후 다 밀리고 난 뒤
			run = set_state_if_time(NORMAL, state_start_time_ + 500 / 2, tm);
			break;

		case DIE:
			// 사망 상태 -> 소멸까지 0.7s 소요
			run = set_state_if_time(ERASED, state_start_time_ + 700, tm);
			break;

		case ERASED:
			return false;
		}

	} while (run);


	return true;
}

void MonsterDuck::draw(Renderer& renderer, const TimeManager& tm)
{
	switch (state_)
	{
	case NORMAL:
		renderer.enroll_texture('M', 0, position_, keep_state_time(tm) / 100, get_flip());
		break;

	case JUMP_READY:
		renderer.enroll_texture('M', 0, position_, keep_state_time(tm) / 50, get_flip());
		break;

	case JUMP:
		renderer.enroll_texture('M', 0, position_, 0, get_flip());
		break;

	case HIT:
	case HIT_KNOCK_BACK:
		renderer.enroll_texture('M', 1, position_, 0, get_flip());
		break;

	case DIE:
		renderer.enroll_texture('M', 2, position_, std::min(3U, keep_state_time(tm) / 100), get_flip());
		break;
	}
}


MonsterDuck::~MonsterDuck()
{
}
