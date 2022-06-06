#pragma once
#include "header.h"

class FieldObject
{
public: 
	enum State
	{
		NORMAL,	// �Ϲ����� ����
		
		LAND, // �� �ٴ��� �հ� ���������� �õ��� ��(��, ���� �浹���� ��)
		      // state_ �ʵ��� ���� LAND�� �Ǹ� �� ��. (set_state�� ���ڷνḸ Ȱ��)
		
		JUMP_READY, JUMP, FLY, FLY_UP, FLY_DOWN, GLIDING, DECEL, TURN,	// Monster�� ����	

		ON_SKILL,									// ��ų ���� ����
		ON_SKILL1, ON_SKILL2, ON_SKILL3, ON_SKILL4, // ��ų ��� ��..

		GRACE_AFTER_SKILL,		// ��ų ��� ���� ���� ����
		GRACE_AFTER_LONG_SKILL,	// �ʻ�� ��� ���� ���� ����
		GRACE_AFTER_HIT,		// �ǰ� ���� ���� ����
		HIT_KNOCK_BACK,			// ���� ����, ��ü�� �з������� ���� ��
		HIT,					// ��ü�� �з��� ����
		DIE,					// FieldObject�� ����� �ϳ� HP�� 0�� �ǵ��� �ؼ� ���� ���
		ERASED					// FieldObject�� �Ҹ�Ǿ�� ��
	};

	enum Direction { LEFT_HEAD, RIGHT_HEAD };


	struct MoveData
	{
		Uint32	time;		// �ش� move�� �۵��� �ð�
		int		ordering;	// �ش� move�� �� ��°�� ���Դ��� (time -> ordering �� ����)

		// (10����) 100�� �ڸ� : add = 0, set = 1 
		// (10����)  10�� �ڸ� : v   = 0, a   = 1
		// (10����)   1�� �ڸ� : all = 0, x   = 1, y = 2
		int		type;

		double	x, y;		// x, y ���� 
	};

protected:
	ObjectManager&	object_manager_;	// �� ��ü�� ��� �ִ� object_manager
	Point<double>	position_;			// ������Ʈ�� ��ġ
	Range<int>		range_;				// ������Ʈ�� (x, y) ���� ����
	
	Direction		direction_;			// ������Ʈ�� �ٶ󺸰� �ִ� ����

	State			state_;
	Uint32			state_start_time_;	// state�� ���������� �ٲ� �ð�

	Vector2d<double>		v_, a_;		// 1ms �� �ӵ�, ���ӵ�

	std::vector<MoveData>	reserved_move_;		// ���� �����ӿ��� ����� ������
	Uint32					last_move_time_;	// move() �Լ����� ���������� ��ġ�� ����� �ð�.

public:
	// �������� �ٶ󺸰� �ִ� ��ü ����
	FieldObject(ObjectManager& parent_object_manager, Uint32 create_time,
		Point<double> position, Range<int> range,
		Vector2d<double> v_px_per_s = { 0, 0 });

	// modify_time �������� state�� �����ϰ�, ���� �� �ؾ��� �۾��� �����Ѵ�.
	// �Ű����� state�� LAND�� ���� state�� �ٲ��� �� ��
	// �Ű����� state�� LAND�� ���� v_, a_ �� ���� ����(�Լ� ������� �� ��) 
	virtual void	set_state(State state, const Uint32 modify_time);

	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾� ����
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm) = 0;

	// ��� ��ü�� �浹���� �� � �۾��� ���� ����
	virtual bool	on_hit(std::weak_ptr<FieldObject> opponent,
		const TimeManager& tm, Renderer& renderer, SoundManager& sm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm) = 0;



	// ���� �ð��� ���� �ð��� �Ѿ�� �� set_state() ȣ��(ȣ�� ���� �� true return)
	bool			set_state_if_time(State state,
		const Uint32 modify_time, const TimeManager& tm);


	// v_, a_�� ���� ���� position_ ���� ����� �����Ѵ�.
	void			move(const Uint32 modify_time);

	// reserved_move�� ����
	// �����ӿ��� ����Ǿ�� �� v_, a_�� �����ϰ� move �Լ��� ����
	void			run_move(const Uint32 modify_time);


	// direction_ ������ LEFT_HEAD��	SDL_FILP_NONE��,
	// direction_ ������ RIGHT_HEAD��	SDL_FILP_HORIZONTAL�� ����
	SDL_RendererFlip get_flip();

	// ������ ���� ������[LEFT_HEAD] (-1), �������� ���� ������[RIGHT_HEAD] 1 return
	int				direction_weight();

	// �ǰ� ����� �� �� ������ true, �ƴϸ� false ����
	virtual bool	get_hittable();

	// ���� ���¸� return
	State			get_state();

	// ��� ��ü�� �浹�ߴ��� �˻�
	bool			is_hit(const FieldObject& opponent);

	// state�� ���������� ����ǰ� �� ms�� ���������� return
	Uint32			keep_state_time(const Uint32 curr_time);

	// ������ �ٲ۴�. ���� �� true, direction�� ����� ���� ������ �� false ����
	// range�� ��Ī��Ų��.
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

