#pragma once
#include "DamageBox.h"

class DamageBoxNormalAttack :
	public DamageBox
{
public:
	DamageBoxNormalAttack(ObjectManager& parent_object_manager,
		Uint32 create_time, double x_position, Vector2d<double> v_per_s, Direction direction);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);
	
	virtual ~DamageBoxNormalAttack();
};

