#pragma once
#include "FieldObject.h"
class Monster :
	public FieldObject
{
protected:
	int hp_;

public:
	Monster(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, Range<int> range, Vector2d<double> v_per_s, int hp);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾��� �����Ѵ�.
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm) = 0;

	// ������ hp_�� amount ��ŭ ���ҽ�Ų��.
	// ���� �� ���� ���� �� true, ��� �� false ����
	bool			damage(const int amount, const Uint32 modify_time, Renderer& renderer);
	
	virtual ~Monster();
};

