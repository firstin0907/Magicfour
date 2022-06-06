#pragma once
#include "AutoDeleteFieldObject.h"

class DamageBox :
	public AutoDeleteFieldObject
{
protected:
	std::map <std::weak_ptr<FieldObject>, Uint32,
		std::owner_less< std::weak_ptr<FieldObject> > > object_hit; // (충돌 상대, 충돌한 시간)

	const double	power_;	// 충돌한 상대를 밀어내는 힘(부딪힌 상대는 v_ * power_의 속도로 밀림)
	const int		damage_;// 적에게 줄 데미지

	int				remain_penetration_; // 남은 관통 횟수. 0이 되면 소멸.

public:
	DamageBox(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, Range<int> range,
		int damage, Vector2d<double> v_per_s = { 0, 0 }, Uint32 life_time = ETERNAL,
		int penetration_number = 1, double power = 0.5);
	
	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
		// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// 상대 객체와 충돌했을 때 어떤 작업을 해줄 건지
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~DamageBox();
};

