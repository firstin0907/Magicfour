#pragma once
#include "AutoDeleteFieldObject.h"
class FontEffect :
	public AutoDeleteFieldObject
{
public:
	enum OPTION
	{
		ALIGN_LEFT = 0x00,
		ALIGN_CENTER = 0x01,
		ALIGN_RIGHT	 = 0x02,
	};
protected:
	std::shared_ptr<Texture>	texture_;

public:
	FontEffect(ObjectManager& parent_object_manager, Uint32 create_time,
		Renderer& renderer, Point<double> position, int font_id, SDL_Color color, 
		std::string content, Vector2d<double> v_per_s = { 0, 0 },
		Uint32 life_time = ETERNAL, FontEffect::OPTION option = ALIGN_CENTER);


	// �����Ӹ��� �����Ͽ� �����Ӹ��� �ʿ��� �۾��� �����Ѵ�.
	// �ش� ��ü�� �Ҹ��� �Ǿ�� �Ѵٸ� false return
	virtual bool	update(const TimeManager& tm);

	// renderer class�� ��ü�� �׸� ������ ������.
	virtual void	draw(Renderer& renderer, const TimeManager& tm);

	virtual ~FontEffect();
};

