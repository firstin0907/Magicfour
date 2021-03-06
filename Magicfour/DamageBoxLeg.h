#pragma once
#include "DamageBox.h"

class DamageBoxLeg :
	public DamageBox
{
public:
	DamageBoxLeg(ObjectManager& parent_object_manager,
		Uint32 create_time, double x_position);

	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBoxLeg();
};

