#pragma once
#include "FieldObject.h"

// Ư�� �ð��� �Ǹ� update �Լ����� ������ false�� return�ϴ�
// FieldObject �ڽ� Ŭ����
class AutoDeleteFieldObject :
	public FieldObject
{
protected:
	// ���� �ð�, �� �ð��� ������ update �Լ����� ������ false�� return
	Uint32	expiry_time_;

public:
	AutoDeleteFieldObject(ObjectManager& parent_object_manager,
		Uint32 create_time, Uint32 life_time,
		Point<double> position, Range<int> range,
		Vector2d<double> v_per_s = { 0, 0 });

	virtual bool	update(const TimeManager& tm);
	virtual void	draw(Renderer& renderer, const TimeManager& tm) = 0;

	virtual ~AutoDeleteFieldObject();
};

