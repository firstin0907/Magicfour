#pragma once
#include "Character.h"
class CharacterOnSkillDuck2 :
	public Character
{
protected:
	Uint32			next_drop_;		// ���� ���ø� ��ô�� �ð�
	int				cnt_;			// ���ݱ��� ��ô�� ������ ��

public:
	CharacterOnSkillDuck2(ObjectManager& parent_object_manager);
		
	// ��ų�� ���۵ż� ���� ĳ����(curr_character)�� �ٲ��� �� ��
	// ĳ���� ��ü(instance)�� ��� ������ �ҷ��´�.
	// ��ų ����� ������ ���� �ؾ��� ���۵� �� �Լ����� ó���Ѵ�.
	virtual void	set_up(const Character* instance);

	// modify_time �������� state�� �����ϰ�, ���� �� �ؾ��� �۾��� �����Ѵ�.
	// �Ű����� state�� LAND�� ���� state�� �ٲ��� �� ��
	// �Ű����� state�� LAND�� ���� v_, a_ �� ���� ����(�Լ� ������� �� ��) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾��� �����Ѵ�.
	// ��ų�� ����Ǿ��� ���� false�� return �Ѵ�.
	virtual bool	update(const TimeManager& tm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~CharacterOnSkillDuck2();
};

