#include "Character.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

constexpr Uint32	SKILL_COOLTIME = 200;
constexpr Uint32	LONG_SKILL_COOLTIME = 1'000;
constexpr Uint32	LONG_SKILL_PUSH_TIME = 1'000;

Character::Character(ObjectManager& parent_object_manager, Uint32 create_time)
	: FieldObject(parent_object_manager, create_time, Point<double>(GAME_W / 2, GROUND_Y), {{-15, -130}, {20, 0}})
{
	speed_ = 240;
	is_left_move_ = is_right_move_ = false;
	skill_available_time_ = skill_use_time_ = 0;
	skill_press_start_ = 0;
	skill_.reserve(4);
	skill_.push_back({ 0, 0 });
	skill_.push_back({ 2, 0 });
}

void Character::set_up(const Character* instance)
{
	position_ = instance->position_;
	range_ = instance->range_;

	direction_ = instance->direction_;
	state_ = instance->state_;
	state_start_time_ = instance->state_start_time_;

	skill_use_time_ = instance->skill_use_time_;

	if (state_ == GRACE_AFTER_LONG_SKILL)
	{
		// LONG SKILL 사용 종료 후 set_up이 호출된 상황
		// 즉, LONG SKILL의 쿨타임을 적용해줘야 함
		skill_end_time_ = state_start_time_;
		skill_available_time_ = skill_end_time_ + LONG_SKILL_COOLTIME;
	}
	else if (state_ == GRACE_AFTER_SKILL)
	{
		// 일반 스킬 사용 종료 후 set_up이 호출된 상황
		// 즉, 일반 SKILL의 쿨타임을 적용해줘야 함
		skill_end_time_ = state_start_time_;
		skill_available_time_ = skill_end_time_ + SKILL_COOLTIME;
	}
	else
	{
		// 스킬을 시작하려고 set_up이 호출된 상황
		// 쿨타임은 일단 무한으로 설정해준 뒤 스킬이 끝나면 끝난 시간에 맞춰 재설정
		skill_end_time_ = 0;
		skill_available_time_ = ETERNAL;
	}

	last_move_time_ = instance->last_move_time_;
	v_ = instance->v_;
	a_ = instance->a_;

	speed_ = instance->speed_;
	is_left_move_ = instance->is_left_move_;
	is_right_move_ = instance->is_right_move_;

	// set_up이 호출된 상황에서는 skill 시전 버튼이 눌렸을 리 없다.
	skill_press_start_ = 0;

	skill_ = instance->skill_;
	reserved_move_ = instance->reserved_move_;
}

void Character::set_state(State state, const Uint32 modify_time)
{
	if (state != LAND) FieldObject::set_state(state, modify_time);

	switch (state)
	{
	case NORMAL:
		break;

	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;

	case HIT:
		if (!skill_.empty())
		{
			remove_skill();
			skill_available_time_ = modify_time;
		}
		else
		{
			set_state(DIE, modify_time);
		}
		break;

	case ON_SKILL:
		skill_use_time_ = modify_time;
		break;

	case DIE:
		set_v({ 0, 0 }, modify_time);
		set_a({ 0, GRAVITY }, modify_time);
		break;
	}
}

bool Character::get_hittable()
{
	switch (state_)
	{
	case HIT:
	case ON_SKILL:
	case ON_SKILL1:
	case ON_SKILL2:
	case ON_SKILL3:
	case ON_SKILL4:
	case GRACE_AFTER_HIT:
	case GRACE_AFTER_SKILL:
	case DIE:
	case ERASED:
		return false;

	default:
		return true;
	}
}

bool Character::update(const TimeManager& tm)
{
	// 캐릭터가 화면 밖으로 나가지 않도록 위치 조정
	if (position_.x < 0) position_.x = 0;
	else if (position_.x > GAME_W) position_.x = GAME_W;
	
	// 2. state_에 따른 동작
	bool run = false;
	do
	{
		run = false;
		switch (state_)
		{
		case NORMAL:
			break;

		case HIT:
			// 피격 직후
			run = set_state_if_time(GRACE_AFTER_HIT, state_start_time_ + 420, tm);
			break;

		case GRACE_AFTER_HIT:
			// 피격 후 무적 시간
			run = set_state_if_time(NORMAL, state_start_time_ + 2'000, tm);
			break;

		case GRACE_AFTER_SKILL:
			// 스킬 사용 후 무적 시간
			run = set_state_if_time(NORMAL, state_start_time_ + 100, tm);
			break;

		case GRACE_AFTER_LONG_SKILL:
			// 필살기 사용 후 무적 시간
			run = set_state_if_time(NORMAL, state_start_time_ + 500, tm);
			break;

		case DIE:
			break;
		}

	} while (run);

	return true;
}

bool Character::on_hit(std::weak_ptr<FieldObject> opponent,
	const TimeManager& tm, Renderer& renderer, SoundManager& sm)
{
	sm.play_sfx(0);
	set_state(HIT, tm.get_curr());
	return true;
}

void Character::draw(Renderer& renderer, const TimeManager& tm)
{
	switch (state_)
	{
	case NORMAL:
		renderer.enroll_texture('C', 0, position_, keep_state_time(tm) / 30, get_flip());
		break;

	case HIT:
		renderer.enroll_texture('C', 2, position_, keep_state_time(tm) / 30, get_flip());
		break;

	case GRACE_AFTER_SKILL:
	case GRACE_AFTER_LONG_SKILL:
		renderer.enroll_texture('C', 4, position_, keep_state_time(tm) / 30, get_flip());
		break;

	case GRACE_AFTER_HIT:
		if ((keep_state_time(tm) / 100) % 2)
		{
			renderer.enroll_texture('C', 0, position_, keep_state_time(tm) / 30, get_flip());
		}
		else
		{
			renderer.enroll_texture('C', 4, position_, keep_state_time(tm) / 30, get_flip());
		}
		break;

	case DIE:
		renderer.enroll_texture('C', 3, position_, std::min(keep_state_time(tm) / 30, 23U), get_flip());
		break;
	}

	// 캐릭터가 죽지 않았을 때 UI 그리기
	draw_ui(renderer, tm);
}

void Character::draw_ui(Renderer& renderer, const TimeManager& tm)
{
	// 죽었을 때는 다른 UI는 띄우지 않고 검은 화면만 띄운다
	if (state_ == DIE)
	{
		if (keep_state_time(tm) >= 1'600)
		{
			renderer.set_color(0, 0, 0, std::min(255U, (keep_state_time(tm) - 1'600) / 10));
			renderer.draw_rect(Range<int>({ 0, 0 }, { 1280, 720 }));
		}
		return;
	}


	// 스킬 게이지 그리기
	if (skill_press_start_)
	{
		// 스킬 쿨타임이 지난 상태에서 스킬 버튼을 누르고 있을 때 무지개 게이지 그리기
		const int gauge_px = std::min(35, static_cast<int>(35 * (tm - skill_press_start_) / LONG_SKILL_PUSH_TIME));

		renderer.enroll_texture('U', 1, Range<int>({ 0, 35 - gauge_px }, { 12, 35 }),
			Range<int>({ 30, -95 - gauge_px }, { 42, -95 }) + position_);
		renderer.draw_range(Range<int>({ 30, -130 }, { 42, -95 }) + position_);
	}
	else if (tm.get_curr() < skill_available_time_)
	{
		// 스킬 쿨타임 중일 때는 회색 게이지 그리기
		const int gauge_px =
			skill_available_time_ == ETERNAL ? 35 :
			std::min(35, static_cast<int>(35 * (skill_available_time_ - tm) / (skill_available_time_ - skill_end_time_)));
		renderer.enroll_texture('U', 2, Range<int>({ 0, 35 - gauge_px }, { 12, 35 }),
			Range<int>({ 30, -95 - gauge_px }, { 42, -95 }) + position_);
		renderer.draw_range(Range<int>({ 30, -130 }, { 42, -95 }) + position_);
	}
	else if (skill_available_time_ <= tm.get_curr() && tm.get_curr() <= skill_available_time_ + 200)
	{
		// 스킬 쿨타임이 막 끝났을 때
		renderer.set_color(255, 255, 255, 255 - (tm - skill_available_time_) * 205 / 200);
		renderer.draw_rect(Range<int>({ 30, -130 }, { 42, -95 }) + position_);
	}


	// 스킬 슬롯 띄우기
	for (int i = 0; i < skill_.size(); i++)
	{
		renderer.enroll_texture('U', 3 + skill_[i].first,
			position_ + Point<int>(-25, -130 + 15 * i), 1, SDL_FLIP_NONE);
		if (tm.get_curr() < skill_[i].second + 250)
		{
			renderer.enroll_texture('U', 8,
				position_ + Point<int>(-25, -130 + 15 * i), 1, SDL_FLIP_NONE);
		}
	}
	for (int i = static_cast<int>(skill_.size()); i < 4; i++)
	{
		renderer.enroll_texture('U', 7, position_ + Point<int>(-25, -130 + 15 * i), 1, SDL_FLIP_NONE);
	}
}

int Character::proceed_input(const SDL_Event& event)
{
	if (state_ == DIE) return -1;

	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_LEFT:
			if (is_left_move_ == false)
			{
				is_left_move_ = true;
				add_v(Vector2d<double>(-speed_, 0), event.key.timestamp);
				change_direction(LEFT_HEAD);
			}
			break;

		case SDL_SCANCODE_RIGHT:
			if (is_right_move_ == false)
			{
				is_right_move_ = true;
				add_v(Vector2d<double>(speed_, 0), event.key.timestamp);
				change_direction(RIGHT_HEAD);
			}
			break;
		case SDL_SCANCODE_Z:
			if (skill_available_time_ <= event.key.timestamp)
			{
				if (skill_press_start_ == 0)
				{
					skill_press_start_ = event.key.timestamp;
				}
			}
		}
		break;

	case SDL_KEYUP:
		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_LEFT:
			if (is_left_move_)
			{
				add_v(Vector2d<double>(speed_, 0), event.key.timestamp);
				is_left_move_ = false;
			}
			break;

		case SDL_SCANCODE_RIGHT:
			if (is_right_move_)
			{
				add_v(Vector2d<double>(-speed_, 0), event.key.timestamp);
				is_right_move_ = false;
			}
			break;

		case SDL_SCANCODE_Z:
			if (skill_press_start_)
			{
				set_state(ON_SKILL, event.key.timestamp);
				
				if (event.key.timestamp - skill_press_start_ <= 1000)
				{
					// 일반 스킬 시전
					skill_press_start_ = 0;
					if (skill_.empty()) return 1;
					else return skill_.back().first * 2 + 2;
				}
				else
				{
					// LONG 스킬 시전
					skill_press_start_ = 0;
					if (skill_.empty()) return 1;
					else return skill_.back().first * 2 + 3;
				}
			};
			break;
		case SDL_SCANCODE_X:
			remove_skill();
			break;
		}
	}
	return -1;
}

bool Character::add_skill(int skill_id, Uint32 modify_time)
{
	if (skill_.size() >= 4) return false;

	skill_.push_back({ skill_id, modify_time });
	return true;
}

bool Character::remove_skill()
{
	if (skill_.empty()) return false;

	skill_.pop_back();
	return true;
}

Character::~Character()
{
}
