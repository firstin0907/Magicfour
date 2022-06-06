#include "Monster.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "FontEffect.h"

Monster::Monster(ObjectManager& parent_object_manager, Uint32 create_time,
	Point<double> position, Range<int> range, Vector2d<double> v_per_s, int hp)
	: FieldObject(parent_object_manager, create_time, position, range, v_per_s), hp_(hp)
{
	change_direction(RandomGenerator::rand(0, 1) ? LEFT_HEAD : RIGHT_HEAD);
	if (direction_ == FieldObject::LEFT_HEAD)
	{
		position_.x = GAME_W - position_.x;
		v_.x = -(v_.x);
	}
}

bool Monster::update(const TimeManager& tm)
{	
	switch (state_)
	{
	case HIT_KNOCK_BACK:
		// 화면 밖으로 밀려나지 않게 위치 조정
		if (position_.x < 0) position_.x = 0;
		else if (position_.x > GAME_W) position_.x = GAME_W;
		break;

	default:
		if (position_.x < 0)
		{
			if (change_direction(FieldObject::RIGHT_HEAD))
			{
				v_.x = -(v_.x);
			}
			position_.x = -position_.x;
		}
		else if (position_.x > GAME_W)
		{
			if (change_direction(FieldObject::LEFT_HEAD))
			{
				v_.x = -(v_.x);
			}
			position_.x = GAME_W - (position_.x - GAME_W);
		}
	}
	
	return true;
}


bool Monster::damage(const int amount, const Uint32 modify_time, Renderer& renderer)
{
	hp_ -= amount;

	object_manager_.insert(std::make_shared<FontEffect>(object_manager_, modify_time,
		renderer, position_ - range_.get_h() / 2 * UNIT_VECTOR2D_Y , 0,
		WHITE, "-" + std::to_string(amount), UNIT_VECTOR2D_Y * (-100), 1500), ObjectManager::NEUTRALS);

	if (hp_ <= 0)
	{
		set_state(DIE, modify_time);
		object_manager_.add_score();
		return false;
	}
	return true;
}

Monster::~Monster()
{

}
