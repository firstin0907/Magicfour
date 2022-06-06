#include "CharacterOnSkillDuck1.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "DamageBoxThorn.h"

constexpr double	GO_UP_V = -GRAVITY * 0.25;

CharacterOnSkillDuck1::CharacterOnSkillDuck1(ObjectManager& parent_object_manager)
	: Character(parent_object_manager, 0)
{
}

void CharacterOnSkillDuck1::set_up(const Character* instance)
{
	Character::set_up(instance);
	add_v({ 0, GO_UP_V }, state_start_time_);
	set_a({ 0, GRAVITY }, state_start_time_);
}


void CharacterOnSkillDuck1::set_state(State state, const Uint32 modify_time)
{
	if(state != LAND) FieldObject::set_state(state, modify_time);
	switch (state)
	{
	case LAND:
		position_.y = GROUND_Y - range_.p2.y;
		v_.y = a_.y = 0;
		break;

	case ON_SKILL1:
		// 고점에 있을 때 캐릭터 멈추기
		set_a({ 0, 0 }, modify_time);
		
		// 데미지 박스 생성
		object_manager_.insert(std::make_shared<DamageBoxThorn<860, 500>>(
			object_manager_, modify_time, position_, 4), ObjectManager::ALLIES);

		object_manager_.insert(std::make_shared<DamageBoxThorn<710, 710>>(
			object_manager_, modify_time, position_, 4), ObjectManager::ALLIES);

		object_manager_.insert(std::make_shared<DamageBoxThorn<0, 710>>(
			object_manager_, modify_time, position_, 4), ObjectManager::ALLIES);

		object_manager_.insert(std::make_shared<DamageBoxThorn<-710, 710>>(
			object_manager_, modify_time, position_, 4), ObjectManager::ALLIES);

		object_manager_.insert(std::make_shared<DamageBoxThorn<-860, 500>>(
			object_manager_, modify_time, position_, 4), ObjectManager::ALLIES);
		break;
		
	case ON_SKILL2:
		// 멈췄던 캐릭터에게 다시 중력 적용시키기
		set_a({ 0, GRAVITY }, modify_time);
		break;
	}
	
}


bool CharacterOnSkillDuck1::update(const TimeManager& tm)
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
			// 스킬 사용 직후 ~ 고점에 올라가기 직전
			run = set_state_if_time(ON_SKILL1, skill_use_time_ + 250, tm);
			break;

		case ON_SKILL1:
			// 고점에 올라가 있는 상태
			run = set_state_if_time(ON_SKILL2, skill_use_time_ + 350, tm);
			break;

		case ON_SKILL2:
			// 고점에 올라갔다가 떨어지고 있는 상태
			return set_state_if_time(GRACE_AFTER_SKILL, skill_use_time_ + 600, tm) == false;
		}
	} while (run);

	return true;
}

void CharacterOnSkillDuck1::draw(Renderer& renderer, const TimeManager& tm)
{	
	// 1. 스킬 사용 후 0.42초가 지나지 않았다면 스킬 모션
	if (tm - skill_use_time_ < 420)
	{
		renderer.enroll_texture('C', 1, position_, (tm - skill_use_time_) / 30, get_flip());
	}
	//     아니면 일반 모션으로!
	else
	{
		renderer.enroll_texture('C', 4, position_, (tm - skill_use_time_) / 30, get_flip());
	}
	draw_ui(renderer, tm);
}

CharacterOnSkillDuck1::~CharacterOnSkillDuck1()
{
}
