#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "component.h"
#include "types.h"
#include "vector.h"
//#include <glib/glist.h>
#include <glib.h>

typedef void (*Window_Callback)(void *data);

typedef struct window_S
{
	Line			name;
	int				id;
	SDL_Rect		rect;
	Bool			hidden;
	Bool			idle;/**<do not update idle windows until refocus*/
	Bool			allow_input;
	Bool			has_focus;
	Component		focus_component;
	int				component_count; /**<how many components are on the window*/
	GList			*components;
	Window_Callback	*callbacks;
	int				callback_count;
	void			*custom_data;
	void			(*draw)(struct Window_S *self);
	Bool			(*update)(struct Window_S *self, GList *updates); /**<handle updates */
}Window;
/**
* @brief initializes the window system 
*/
void window_init();

void window_config();

void window_clean();

void window_update_all();

Window * window_load_from_file_to(Line file, Vec2d position);

Window * window_load_from_file(Line file);

Window * window_new();

void window_move_to(Window * win, Vec2d position);

void window_free(Window **win);

Bool window_mouse_outside(Window *win);

Bool window_mouse_inside(Window *win);

void window_add_component(Window *win, Component * component);

Component *window_get_component_by_id(Window *win, int id);

Component *window_get_component_by_name(Window *win, Line name);

int window_get_refcount(Window * win);

void window_hide(Window *win);

void window_show(Window *win);

void window_idle(Window *win);

void window_resume(Window *win);
#endif