#pragma once
#include "DamageBox.h"
class DamageBoxBead :
	public DamageBox
{
public:
	DamageBoxBead(ObjectManager& parent_object_manager,
		Uint32 create_time, Point<double> position, Vector2d<double> v_per_s, int damage);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// ��� ��ü�� �浹���� �� � �۾��� ���� ���� (�浹 ���� �� true return)
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBoxBead();
};

