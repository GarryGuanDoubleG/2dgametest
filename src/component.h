#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include "types.h"
#include "vector.h"

enum ComponentTypes
{
	NullComponent		= 0,
	LabelComponent		= 1,
	ButtonComponent		= 2,
	EntryComponent		= 3,
	SliderComponent		= 4,
	ImageComponent		= 5,
	ActorComponent		= 6,
	ListComponent		= 7,
	PercentBarComponent = 8,
	CheckComponent      = 9
};

enum ComponentStates
{
	CheckBadState		= -1,
	CheckChecked		= 0,
	CheckUnchecked		= 1
};

typedef struct Component_S
{
	unsigned int		id;
	Line				name;
	Rect_f				rect;
	SDL_Rect			bounds;
	SDL_Rect			canvas;
	Bool				canvas_has_focus;
	Bool				has_focus;
	int					state;
	int					old_state;
	int					type;
	Bool				hidden;
	void				*component_data;
	void				(*data_free)(struct Component_S *component);
	void				(*data_draw)(struct Component_S *component);
	void				(*data_move)(struct Component_S * component, SDL_Rect bounds);
	Bool				(*data_update)(struct Component_S *component);
	int					(*data_get_state)(struct Component_S *component);
	Bool				(*data_changed)(struct Component_S *component);
}Component;

#endif