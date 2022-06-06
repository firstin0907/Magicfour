#include "FieldObject.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "FieldObject.h"
#include "TimeManager.h"
#include "SoundManager.h"


FieldObject::FieldObject(ObjectManager& parent_object_manager, Uint32 create_time,
	Point<double> position, Range<int> range, Vector2d<double> v_px_per_s)
	: object_manager_(parent_object_manager), position_(position), range_(range), v_(v_px_per_s * 0.001)
{
	direction_ = RIGHT_HEAD;
	state_ = NORMAL;
	state_start_time_ = create_time;
	last_move_time_ = create_time;
	a_ = Vector2d<double>(0, 0);
}

void FieldObject::set_state(State state, const Uint32 modify_time)
{
	if (state_start_time_ <= modify_time)
	{
		state_ = state;
		state_start_time_ = modify_time;
	}
}

int FieldObject::direction_weight()
{
	return direction_ == LEFT_HEAD ? -1 : 1;
}

bool FieldObject::on_hit(std::weak_ptr<FieldObject> opponent, const TimeManager& tm,
	Renderer& renderer, SoundManager& sm)
{
	return false;
}

bool FieldObject::set_state_if_time(State state, const Uint32 modify_time, const TimeManager& tm)
{
	if (modify_time <= tm.get_curr())
	{
		set_state(state, modify_time);
		return true;
	}
	else
	{
		return false;
	}
}

void FieldObject::move(const Uint32 modify_time)
{
	const double limit_y =
		GROUND_Y - ((range_.type == Range<int>::RANGE_CIRCLE) ? range_.r : range_.p2.y);

	const Uint32 delta_time = modify_time - last_move_time_;

	if (modify_time < last_move_time_)
	{
		// error
		return;
	}

	Point<double> position_after_move = position_ + (2 * v_ + a_ * delta_time) * 0.5 * delta_time;

	// 땅에 착지했을 때 (공중 or 땅에서 -> 지하로 이동 시도)
	if (position_.y <= limit_y && limit_y < position_after_move.y)
	{
		const double s = limit_y - position_.y;

		const double delta_time_before_land =
			a_.y ? ((-v_.y + sqrt(v_.y * v_.y + 2 * a_.y * s)) / a_.y) : (s / v_.y);
		const double delta_time_after_land = delta_time - delta_time_before_land;

		position_ += (2 * v_ + a_ * delta_time_before_land) * 0.5 * delta_time_before_land;

		set_state(LAND, last_move_time_ + static_cast<Uint32>(delta_time_before_land));

		position_ += (2 * v_ + a_ * delta_time_after_land) * 0.5 * delta_time_after_land;
	}
	// 땅에 부딪히지 않았을 때(땅에 계속 있었거나 공중에 있거나)
	else
	{
		position_ = position_after_move;
		v_ += a_ * delta_time;
	}

	last_move_time_ = modify_time;
}

void FieldObject::run_move(const Uint32 modify_time)
{
	// time -> ordering 순으로 정렬
	std::sort(reserved_move_.begin(), reserved_move_.end(), [](MoveData& a, MoveData& b)
	{
		if (a.time != b.time) return a.time < b.time;
		else return a.ordering < b.ordering;
	});

	for (auto &[time, _, type, x, y] : reserved_move_)
	{
		move(time);

		switch (type)
		{
			// add
		case 0: case 1: case 2:
			v_.x += x, v_.y += y; break;
		case 10: case 11: case 12:
			a_.x += x, a_.y += y; break;

			// set v
		case 100:
			v_.x = x; v_.y = y;  break;
		case 101:
			v_.x = x; break;
		case 102:
			v_.y = y; break;

			// set a
		case 110:
			a_.x = x; a_.y = y;  break;
		case 111:
			a_.x = x; break;
		case 112:
			a_.y = y; break;
		}
	}
	move(modify_time);
	reserved_move_.clear();
}

SDL_RendererFlip FieldObject::get_flip()
{
	return (direction_ == LEFT_HEAD) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

bool FieldObject::get_hittable()
{
	switch (state_)
	{
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

FieldObject::State FieldObject::get_state()
{
	return state_;
}

bool FieldObject::is_hit(const FieldObject& opponent)
{
	return (range_ + position_).is_overlaped(opponent.range_ + opponent.position_);
}

Uint32 FieldObject::keep_state_time(const Uint32 curr_time)
{
	if (curr_time < state_start_time_) return 0;
	else return curr_time - state_start_time_;
}

bool FieldObject::change_direction(Direction direction)
{
	if (direction_ != direction)
	{
		std::swap(range_.p1.x, range_.p2.x);
		range_.p1.x = -range_.p1.x;
		range_.p2.x = -range_.p2.x;
		direction_ = direction;
		return true;
	}
	else
	{
		return false;
	}
	
}

void FieldObject::add_v(Vector2d<double> px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 0, px_per_s.x, px_per_s.y });
}

void FieldObject::add_v_x(double px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 1, px_per_s, 0 });
}

void FieldObject::add_v_y(double px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 2, 0, px_per_s });
}

void FieldObject::add_a(Vector2d<double> px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 10, px_per_s2.x, px_per_s2.y });
}

void FieldObject::add_a_x(double px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 11, px_per_s2, 0});
}

void FieldObject::add_a_y(double px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 12, 0, px_per_s2 });
}

void FieldObject::set_v(Vector2d<double> px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 100, px_per_s.x, px_per_s.y });
}

void FieldObject::set_v_x(double px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 101, px_per_s, 0 });
}

void FieldObject::set_v_y(double px_per_s, Uint32 modify_time)
{
	px_per_s = px_per_s * 0.001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 102, 0, px_per_s });
}

void FieldObject::set_a(Vector2d<double> px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 110, px_per_s2.x, px_per_s2.y });
}

void FieldObject::set_a_x(double px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 111, px_per_s2, 0 });
}

void FieldObject::set_a_y(double px_per_s2, Uint32 modify_time)
{
	px_per_s2 = px_per_s2 * 0.000001;
	reserved_move_.push_back({ modify_time, static_cast<int>(reserved_move_.size()), 112, 0, px_per_s2 });
}


FieldObject::~FieldObject()
{
}
