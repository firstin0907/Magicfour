#include "ObjectManager.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "MonsterBird.h"
#include "MonsterDuck.h"
#include "MonsterOctopus.h"

#include "CharacterOnAttack.h"
#include "CharacterOnSkillBird1.h"
#include "CharacterOnSkillBird2.h"
#include "CharacterOnSkillDuck1.h"
#include "CharacterOnSkillDuck2.h"
#include "CharacterOnSkillOctopus1.h"
#include "CharacterOnSkillOctopus2.h"

ObjectManager::ObjectManager()
	: score_(0)
{
	character_on_skill.push_back(std::make_shared<Character>(*this, 0));
	character_on_skill.push_back(std::make_shared<CharacterOnAttack>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillDuck1>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillDuck2>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillOctopus1>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillOctopus2>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillBird1>(*this));
	character_on_skill.push_back(std::make_shared<CharacterOnSkillBird2>(*this));
	
	curr_character_ = character_on_skill[0];
}

void ObjectManager::start_spon(Uint32 spon_start_time)
{
	spon_start_time_ = spon_start_time;
	next_create_ = spon_start_time;
	spon_term_ = 8'000;
}

void ObjectManager::move(const TimeManager& tm)
{
	curr_character_->run_move(tm);

	for (auto i = allies.begin(); i != allies.end(); ++i)
	{
		(*i)->run_move(tm);
	}
	
	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		(*i)->run_move(tm);
	}
	
	for (auto i = neutrals.begin(); i != neutrals.end(); ++i)
	{
		(*i)->run_move(tm);
	}
}

void ObjectManager::draw(Renderer& renderer, const TimeManager& tm)
{
	
	
	for (auto i = allies.begin(); i != allies.end(); ++i)
	{
		(*i)->draw(renderer, tm);
	}

	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		(*i)->draw(renderer, tm);
	}

	for (auto i = neutrals.begin(); i != neutrals.end(); ++i)
	{
		(*i)->draw(renderer, tm);
	}

	curr_character_->draw(renderer, tm);

	if (score_texture_ == nullptr)
	{
		score_texture_ = renderer.make_text("score_ : " + std::to_string(score_), 0, WHITE);
		score_texture_->center.x = score_texture_->size.x + 10;
		score_texture_->center.y = score_texture_->size.y + 10;
	}
	renderer.enroll_texture(score_texture_, { 1280, 720 }, 0, SDL_FLIP_NONE);
}

void ObjectManager::update(const TimeManager& tm, Renderer& renderer, SoundManager& sm) 
{
	while (next_create_ <= tm.get_curr())
	{
		// 몬스터를 소환할 때가 되었으면 몬스터 소환
		int monster_species = RandomGenerator::rand(0, 2);
		switch (monster_species)
		{
		case 0:
			insert(std::make_shared<MonsterOctopus>(*this, tm.get_curr()), ObjectManager::ENEMIES);
			break;
		case 1:
			insert(std::make_shared<MonsterDuck>(*this, tm.get_curr()), ObjectManager::ENEMIES);
			break;
		case 2:
			insert(std::make_shared<MonsterBird>(*this, tm.get_curr()), ObjectManager::ENEMIES);
			break;
		}
		if ((next_create_ - spon_start_time_) / 30'000 < (next_create_ - spon_start_time_ + spon_term_) / 30'000)
		{
			// 30초가 지날 때마다 몬스터 생성 간격을 90%로 줄임.
			spon_term_ = spon_term_ * 9 / 10;
		}
		next_create_ += spon_term_;
	}

	// 캐릭터의 업데이트 처리
	if (curr_character_->update(tm) == false)
	{
		// 캐릭터의 스킬이 종료되었다면 캐릭터 전환
		change_character(0);
	}

	// 아군의 업데이트 처리
	for (auto i = allies.begin(); i != allies.end(); )
	{
		if ((*i)->update(tm) == false) i = allies.erase(i);
		else ++i;
	}

	// 적군의 업데이트 처리
	for (auto i = enemies.begin(); i != enemies.end();)
	{
		if ((*i)->update(tm) == false) i = enemies.erase(i);
		else ++i;
	}

	// 중립의 업데이트 처리
	for (auto i = neutrals.begin(); i != neutrals.end();)
	{
		if ((*i)->update(tm) == false) i = neutrals.erase(i);
		else ++i;
	}


	// [캐릭터 <-> 적] 간 충돌 처리
	for (auto j = enemies.begin(); j != enemies.end(); ++j)
	{
		if (curr_character_->get_hittable() && (*j)->get_hittable() && curr_character_->is_hit(**j))
		{
			curr_character_->on_hit(*j, tm, renderer, sm);
			(*j)->on_hit(curr_character_, tm, renderer, sm);
		}
	}

	// [캐릭터 외 아군 <-> 적] 간 충돌 처리
	for (auto i = allies.begin(); i != allies.end(); ++i)
	{
		if ((*i)->get_hittable() == false) continue;

		for (auto j = enemies.begin(); j != enemies.end(); ++j)
		{
			if ((*j)->get_hittable() == false) continue;

			if ((*i)->is_hit(**j))
			{
				(*i)->on_hit((*j), tm, renderer, sm);
				(*j)->on_hit((*i), tm, renderer, sm);
			}
		}
	}
}

void ObjectManager::change_character(int number)
{
	character_on_skill[number]->set_up(curr_character_.get());
	curr_character_->set_state(FieldObject::ERASED, 0);
	curr_character_ = character_on_skill[number];
}

void ObjectManager::add_score(int val)
{
	score_ += val;
	score_texture_ = nullptr;
}

int ObjectManager::get_score()
{
	return score_;
}



void ObjectManager::insert(std::shared_ptr<FieldObject> field_object, ListName insert_target)
{
	switch (insert_target)
	{
	case ALLIES:
		allies.insert(field_object); break;

	case ENEMIES:
		enemies.insert(field_object); break;

	case NEUTRALS:
		neutrals.insert(field_object); break;
	}
}

ObjectManager::~ObjectManager()
{
}
