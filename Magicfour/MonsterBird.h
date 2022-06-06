#pragma once
#include "Monster.h"
class MonsterBird :
	public Monster
{
private:
	int				y_destination;
	Uint32			next_move_;
public:
	static const int max_hp_ = 20;

public:
	MonsterBird(ObjectManager& parent_object_manager, Uint32 create_time);
	
	// modify_time 기준으로 state를 변경하고, 변경 시 해야할 작업을 수행한다.
	// 매개변수 state가 LAND일 때는 state값 바꾸지 말 것
	// 매개변수 state가 LAND일 떄는 v_, a_ 값 직접 변경(함수 사용하지 말 것) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~MonsterBird();
};

