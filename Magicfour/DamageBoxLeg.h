#pragma once
#include "DamageBox.h"

class DamageBoxLeg :
	public DamageBox
{
public:
	DamageBoxLeg(ObjectManager& parent_object_manager,
		Uint32 create_time, double x_position);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBoxLeg();
};

