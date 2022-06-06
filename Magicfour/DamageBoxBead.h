#pragma once
#include "DamageBox.h"
class DamageBoxBead :
	public DamageBox
{
public:
	DamageBoxBead(ObjectManager& parent_object_manager,
		Uint32 create_time, Point<double> position, Vector2d<double> v_per_s, int damage);

	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// 상대 객체와 충돌했을 때 어떤 작업을 해줄 건지 (충돌 성공 시 true return)
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBoxBead();
};

