#pragma once
#include "FieldObject.h"

class Character :
	public FieldObject
{
protected:
	double		speed_;								// 캐릭터의 x방향 속도

	bool		is_left_move_, is_right_move_;		// 왼쪽, 오른쪽 화살표가 눌려 있는가?
	Uint32		skill_press_start_;					// Z키가 눌리기 시작한 시간

	std::vector<std::pair<int, Uint32> > skill_;	// 캐릭터가 가지고 있는 스킬이다.(최대 4개)
	
	Uint32		skill_use_time_;			// 스킬이 시작된 시간
	Uint32		skill_end_time_;			// 스킬이 종료된 시간
	Uint32		skill_available_time_;		// 스킬을 다시 쓸 수 있는 시간

public:
	Character(ObjectManager& parent_object_manager, Uint32 create_time);

	// 스킬이 시작되거나 종료되어, 현재 캐릭터(curr_character)가 바뀌어야 할 때
	// 캐릭터 객체(instance)의 모든 정보를 불러온다.
	// 스킬 사용이 끝난 직후 (CharacterOn~~ 함수에서는 시작한 후)
	// 해야할 동작도 이 함수에서 처리한다.
	virtual void	set_up(const Character* instance);

	// modify_time 기준으로 state를 변경하고, 변경 시 해야할 작업을 수행한다.
	// 매개변수 state가 LAND일 때는 state값 바꾸지 말 것
	// 매개변수 state가 LAND일 떄는 v_, a_ 값 직접 변경(함수 사용하지 말 것) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// 프레임마다 실행하여 프레임마다 필요한 작업을 수행한다.
	// Character 함수에서는 스킬이 종료되었을 때만 false를 return 한다.
	virtual bool	update(const TimeManager& tm);

	// 상대 객체와 충돌했을 때 어떤 작업을 해줄 건지
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	// 스킬 슬롯, 스킬 게이지, (게임 종료 시) 검은 화면 등을 출력한다.
	void			draw_ui(Renderer& renderer, const TimeManager& tm);

	// 피격 대상이 될 수 있으면 true, 아니면 false 리턴
	virtual bool	get_hittable();

	// KEY_DOWN, KEY_UP 이벤트가 들어왔을 때 처리를 해준다.
	// 스킬을 사용해야 한다면(올바른 조건에서 Z키를 땠을 때) 사용할 스킬 번호를, 아니면 -1 return
	int				proceed_input(const SDL_Event& event);

	// 가장 최근에 획득한 스킬을 삭제한다.
	// 스킬이 가득 차 있을 때(4개 소지 시) false, 스킬 추가에 성공 시 true를 리턴한다.
	bool			add_skill(int skill_id, Uint32 modify_time);

	// 가장 최근에 획득한 스킬을 삭제한다.
	// 스킬이 아예 없을 때 false, 스킬 삭제에 성공 시 true를 리턴한다.
	bool			remove_skill();


	virtual ~Character();
};

