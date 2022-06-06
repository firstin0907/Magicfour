#pragma once
#include "FieldObject.h"

class Character :
	public FieldObject
{
protected:
	double		speed_;								// ĳ������ x���� �ӵ�

	bool		is_left_move_, is_right_move_;		// ����, ������ ȭ��ǥ�� ���� �ִ°�?
	Uint32		skill_press_start_;					// ZŰ�� ������ ������ �ð�

	std::vector<std::pair<int, Uint32> > skill_;	// ĳ���Ͱ� ������ �ִ� ��ų�̴�.(�ִ� 4��)
	
	Uint32		skill_use_time_;			// ��ų�� ���۵� �ð�
	Uint32		skill_end_time_;			// ��ų�� ����� �ð�
	Uint32		skill_available_time_;		// ��ų�� �ٽ� �� �� �ִ� �ð�

public:
	Character(ObjectManager& parent_object_manager, Uint32 create_time);

	// ��ų�� ���۵ǰų� ����Ǿ�, ���� ĳ����(curr_character)�� �ٲ��� �� ��
	// ĳ���� ��ü(instance)�� ��� ������ �ҷ��´�.
	// ��ų ����� ���� ���� (CharacterOn~~ �Լ������� ������ ��)
	// �ؾ��� ���۵� �� �Լ����� ó���Ѵ�.
	virtual void	set_up(const Character* instance);

	// modify_time �������� state�� �����ϰ�, ���� �� �ؾ��� �۾��� �����Ѵ�.
	// �Ű����� state�� LAND�� ���� state�� �ٲ��� �� ��
	// �Ű����� state�� LAND�� ���� v_, a_ �� ���� ����(�Լ� ������� �� ��) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾��� �����Ѵ�.
	// Character �Լ������� ��ų�� ����Ǿ��� ���� false�� return �Ѵ�.
	virtual bool	update(const TimeManager& tm);

	// ��� ��ü�� �浹���� �� � �۾��� ���� ����
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	// ��ų ����, ��ų ������, (���� ���� ��) ���� ȭ�� ���� ����Ѵ�.
	void			draw_ui(Renderer& renderer, const TimeManager& tm);

	// �ǰ� ����� �� �� ������ true, �ƴϸ� false ����
	virtual bool	get_hittable();

	// KEY_DOWN, KEY_UP �̺�Ʈ�� ������ �� ó���� ���ش�.
	// ��ų�� ����ؾ� �Ѵٸ�(�ùٸ� ���ǿ��� ZŰ�� ���� ��) ����� ��ų ��ȣ��, �ƴϸ� -1 return
	int				proceed_input(const SDL_Event& event);

	// ���� �ֱٿ� ȹ���� ��ų�� �����Ѵ�.
	// ��ų�� ���� �� ���� ��(4�� ���� ��) false, ��ų �߰��� ���� �� true�� �����Ѵ�.
	bool			add_skill(int skill_id, Uint32 modify_time);

	// ���� �ֱٿ� ȹ���� ��ų�� �����Ѵ�.
	// ��ų�� �ƿ� ���� �� false, ��ų ������ ���� �� true�� �����Ѵ�.
	bool			remove_skill();


	virtual ~Character();
};

