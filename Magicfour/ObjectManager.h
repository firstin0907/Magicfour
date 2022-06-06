#pragma once
#include "header.h"

class ObjectManager
{

public:
	std::shared_ptr<Character>	curr_character_;

private:
	Uint32	spon_start_time_;	// 소환을 시작한 시간 (= 게임이 시작된 시간)
	Uint32	next_create_;		// 다음 몬스터를 소환할 시간
	Uint32	spon_term_;			// 몇 ms 당 몬스터를 1마리 소환할 것인지

	Unordered_list<std::shared_ptr<FieldObject> >	allies;
	Unordered_list<std::shared_ptr<FieldObject> >	enemies;
	Unordered_list<std::shared_ptr<FieldObject> >	neutrals;

	// Character와 CharacterOnSkill 객체를 하나씩 집어넣는다.
	std::vector<std::shared_ptr<Character> >		character_on_skill;

	int							score_;			// 처치한 적의 수
	std::shared_ptr<Texture>	score_texture_;	// score_를 띄울 텍스쳐

public:

	enum ListName { ALLIES, ENEMIES, NEUTRALS };

	ObjectManager();

	// spon을 시작하기 위해서 관련 변수 초기화
	void start_spon(Uint32 spon_start_time);

	// allies, enemies, neutrals의 모든 원소들 update(~~) 실행
	void move(const TimeManager& tm);

	// allies, enemies, neutrals의 모든 원소들 update(~~) 실행
	void draw(Renderer& renderer, const TimeManager& tm);

	// allies, enemies, neutrals의 모든 원소들 update(~~) 실행
	void update(const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// 스킬 사용, 종료를 위해서 curr_character를 바꾼다.
	void change_character(int number);

	// 점수를 올린다.
	void add_score(int val = 1);
	
	// 점수를 return 
	int get_score();

	// vector에 원소를 삽입한다.
	void insert(std::shared_ptr<FieldObject> field_object, ListName insert_target);

	~ObjectManager();
};

