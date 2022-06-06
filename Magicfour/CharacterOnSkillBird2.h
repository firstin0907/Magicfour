#pragma once
#include "Character.h"
class CharacterOnSkillBird2 :
	public Character
{
private:
	// 다음 DamageBox를 만들 시간
	Uint32			next_create_;

	// 지금까지 DamageBox를 만든 횟수
	int				cnt_;

public:
	CharacterOnSkillBird2(ObjectManager& parent_object_manage);

	// 스킬이 시작돼서 현재 캐릭터(curr_character)가 바뀌어야 할 때
	// 캐릭터 객체(instance)의 모든 정보를 불러온다.
	// 스킬 사용이 시작한 직후 해야할 동작도 이 함수에서 처리한다.
	virtual void	set_up(const Character* instance);

	// modify_time 기준으로 state를 변경하고, 변경 시 해야할 작업을 수행한다.
	// 매개변수 state가 LAND일 때는 state값 바꾸지 말 것
	// 매개변수 state가 LAND일 떄는 v_, a_ 값 직접 변경(함수 사용하지 말 것) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// 프레임마다 실행하여 프레임마다 필요한 작업을 수행한다.
	// 스킬이 종료되었을 때만 false를 return 한다.
	virtual bool	update(const TimeManager& tm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);


	virtual ~CharacterOnSkillBird2();
};

