#pragma once
#include "FieldObject.h"

// 특정 시간이 되면 update 함수에서 무조건 false만 return하는
// FieldObject 자식 클래스
class AutoDeleteFieldObject :
	public FieldObject
{
protected:
	// 기준 시간, 이 시간이 지나면 update 함수에서 무조건 false만 return
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

