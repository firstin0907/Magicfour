#pragma once
#include "header.h"

class ObjectManager
{

public:
	std::shared_ptr<Character>	curr_character_;

private:
	Uint32	spon_start_time_;	// ��ȯ�� ������ �ð� (= ������ ���۵� �ð�)
	Uint32	next_create_;		// ���� ���͸� ��ȯ�� �ð�
	Uint32	spon_term_;			// �� ms �� ���͸� 1���� ��ȯ�� ������

	Unordered_list<std::shared_ptr<FieldObject> >	allies;
	Unordered_list<std::shared_ptr<FieldObject> >	enemies;
	Unordered_list<std::shared_ptr<FieldObject> >	neutrals;

	// Character�� CharacterOnSkill ��ü�� �ϳ��� ����ִ´�.
	std::vector<std::shared_ptr<Character> >		character_on_skill;

	int							score_;			// óġ�� ���� ��
	std::shared_ptr<Texture>	score_texture_;	// score_�� ��� �ؽ���

public:

	enum ListName { ALLIES, ENEMIES, NEUTRALS };

	ObjectManager();

	// spon�� �����ϱ� ���ؼ� ���� ���� �ʱ�ȭ
	void start_spon(Uint32 spon_start_time);

	// allies, enemies, neutrals�� ��� ���ҵ� update(~~) ����
	void move(const TimeManager& tm);

	// allies, enemies, neutrals�� ��� ���ҵ� update(~~) ����
	void draw(Renderer& renderer, const TimeManager& tm);

	// allies, enemies, neutrals�� ��� ���ҵ� update(~~) ����
	void update(const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// ��ų ���, ���Ḧ ���ؼ� curr_character�� �ٲ۴�.
	void change_character(int number);

	// ������ �ø���.
	void add_score(int val = 1);
	
	// ������ return 
	int get_score();

	// vector�� ���Ҹ� �����Ѵ�.
	void insert(std::shared_ptr<FieldObject> field_object, ListName insert_target);

	~ObjectManager();
};

