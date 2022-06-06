#pragma once
#include "AutoDeleteFieldObject.h"

class DamageBox :
	public AutoDeleteFieldObject
{
protected:
	std::map <std::weak_ptr<FieldObject>, Uint32,
		std::owner_less< std::weak_ptr<FieldObject> > > object_hit; // (�浹 ���, �浹�� �ð�)

	const double	power_;	// �浹�� ��븦 �о�� ��(�ε��� ���� v_ * power_�� �ӵ��� �и�)
	const int		damage_;// ������ �� ������

	int				remain_penetration_; // ���� ���� Ƚ��. 0�� �Ǹ� �Ҹ�.

public:
	DamageBox(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, Range<int> range,
		int damage, Vector2d<double> v_per_s = { 0, 0 }, Uint32 life_time = ETERNAL,
		int penetration_number = 1, double power = 0.5);
	
	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
		// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// ��� ��ü�� �浹���� �� � �۾��� ���� ����
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBox();
};

