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

	// 프레임마다 실행하여 프레임마다 필요한 작업을 수행한다.
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm) = 0;

	// 몬스터의 hp_를 amount 만큼 감소시킨다.
	// 감소 후 몬스터 생존 시 true, 사망 시 false 리턴
	bool			damage(const int amount, const Uint32 modify_time, Renderer& renderer);
	
	virtual ~Monster();
};

