#pragma once
#include "header.h"

class FieldObject
{
public: 
	enum State
	{
		NORMAL,	// 일반적인 상태
		
		LAND, // 땅 바닥을 뚫고 내려가려고 시도할 때(즉, 땅과 충돌했을 때)
		      // state_ 필드의 값이 LAND가 되면 안 됨. (set_state의 인자로써만 활용)
		
		JUMP_READY, JUMP, FLY, FLY_UP, FLY_DOWN, GLIDING, DECEL, TURN,	// Monster의 상태	

		ON_SKILL,									// 스킬 시작 직후
		ON_SKILL1, ON_SKILL2, ON_SKILL3, ON_SKILL4, // 스킬 사용 중..

		GRACE_AFTER_SKILL,		// 스킬 사용 직후 무적 상태
		GRACE_AFTER_LONG_SKILL,	// 필살기 사용 직후 무적 상태
		GRACE_AFTER_HIT,		// 피격 직후 무적 상태
		HIT_KNOCK_BACK,			// 맞은 직후, 객체가 밀려나가고 있을 때
		HIT,					// 객체가 밀려난 이후
		DIE,					// FieldObject가 존재는 하나 HP가 0이 되든지 해서 죽은 경우
		ERASED					// FieldObject가 소멸되어야 함
	};

	enum Direction { LEFT_HEAD, RIGHT_HEAD };


	struct MoveData
	{
		Uint32	time;		// 해당 move가 작동할 시간
		int		ordering;	// 해당 move가 몇 번째로 들어왔는지 (time -> ordering 순 정렬)

		// (10진법) 100의 자리 : add = 0, set = 1 
		// (10진법)  10의 자리 : v   = 0, a   = 1
		// (10진법)   1의 자리 : all = 0, x   = 1, y = 2
		int		type;

		double	x, y;		// x, y 인자 
	};

protected:
	ObjectManager&	object_manager_;	// 이 객체를 담고 있는 object_manager
	Point<double>	position_;			// 오브젝트의 위치
	Range<int>		range_;				// 오브젝트의 (x, y) 기준 범위
	
	Direction		direction_;			// 오브젝트가 바라보고 있는 방향

	State			state_;
	Uint32			state_start_time_;	// state가 마지막으로 바뀐 시간

	Vector2d<double>		v_, a_;		// 1ms 당 속도, 가속도

	std::vector<MoveData>	reserved_move_;		// 현재 프레임에서 예약된 움직임
	Uint32					last_move_time_;	// move() 함수에서 마지막으로 위치가 적용된 시간.

public:
	// 오른쪽을 바라보고 있는 객체 생성
	FieldObject(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, Range<int> range,
		Vector2d<double> v_px_per_s = { 0, 0 });

	// modify_time 기준으로 state를 변경하고, 변경 시 해야할 작업을 수행한다.
	// 매개변수 state가 LAND일 때는 state값 바꾸지 말 것
	// 매개변수 state가 LAND일 떄는 v_, a_ 값 직접 변경(함수 사용하지 말 것) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// 프레임마다 실행하여 프레임마다 필요한 작업 수행
	// 해당 객체가 소멸이 되어야 한다면 false return
	virtual bool	update(const TimeManager& tm) = 0;

	// 상대 객체와 충돌했을 때 어떤 작업을 해줄 건지
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class로 객체를 그릴 정보를 보낸다.
	virtual void	draw(Renderer& renderer, const TimeManager& tm) = 0;



	// 현재 시간이 변경 시간을 넘어섰을 때 set_state() 호출(호출 성공 시 true return)
	bool			set_state_if_time(State state,
		const Uint32 modify_time, const TimeManager& tm);


	// v_, a_의 값에 따라 position_ 값을 계산해 적용한다.
	void			move(const Uint32 modify_time);

	// reserved_move에 따라서
	// 프레임에서 변경되어야 할 v_, a_를 변경하고 move 함수를 실행
	void			run_move(const Uint32 modify_time);


	// direction_ 변수가 LEFT_HEAD면	SDL_FILP_NONE을,
	// direction_ 변수가 RIGHT_HEAD면	SDL_FILP_HORIZONTAL을 리턴
	SDL_RendererFlip get_flip();

	// 왼쪽을 보고 있으면[LEFT_HEAD] (-1), 오른쪽을 보고 있으면[RIGHT_HEAD] 1 return
	int				direction_weight();

	// 피격 대상이 될 수 있으면 true, 아니면 false 리턴
	virtual bool	get_hittable();

	// 현재 상태를 return
	State			get_state();

	// 상대 객체와 충돌했는지 검사
	bool			is_hit(const FieldObject& opponent);

	// state가 마지막으로 변경되고 몇 ms가 지났는지를 return
	Uint32			keep_state_time(const Uint32 curr_time);

	// 방향을 바꾼다. 성공 시 true, direction이 현재와 같은 방향일 시 false 리턴
	// range도 대칭시킨다.
	bool			change_direction(Direction direction);
	
	void			add_v(Vector2d<double> px_per_s, Uint32 modify_time);
	void			add_v_x(double px_per_s, Uint32 modify_time);
	void			add_v_y(double px_per_s, Uint32 modify_time);

	void			add_a(Vector2d<double> px_per_s2, Uint32 modify_time);
	void			add_a_x(double px_per_s2, Uint32 modify_time);
	void			add_a_y(double px_per_s2, Uint32 modify_time);

	void			set_v(Vector2d<double> px_per_s, Uint32 modify_time);
	void			set_v_x(double px_per_s, Uint32 modify_time);
	void			set_v_y(double px_per_s, Uint32 modify_time);

	void			set_a(Vector2d<double> px_per_s2, Uint32 modify_time);
	void			set_a_x(double px_per_s2, Uint32 modify_time);
	void			set_a_y(double px_per_s2, Uint32 modify_time);


	virtual ~FieldObject();
};

