#include "HUD.h"

Sprite2 * HUD_bar;
Sprite2 * HUD_menu;
Sprite2 * HUD_item_bag;

static HUD_state hud_state_curr;

void hud_init()
{
	hud_state_curr = main_menu;

	HUD_bar = sprite_load(HEALTH_MANA_BAR_PATH, BAR_IMG_W, BAR_IMG_H, BAR_FRAME_W, BAR_FRAME_H);
	HUD_menu = sprite_load(HUD_MENU_GRID_PATH, HUD_MENU_IMG_W, HUD_MENU_IMG_H, HUD_MENU_FRAME_W, HUD_MENU_FRAME_H);
	HUD_item_bag = sprite_load(HUD_ITEM_BAG_PATH, HUD_ITEM_BAG_IMG_W, HUD_ITEM_BAG_IMG_H, HUD_ITEM_BAG_FRAME_W, HUD_ITEM_BAG_FRAME_H);
}


void hud_draw(SDL_Rect camera, float health, float max_health, float mana, float max_mana)
{

	float health_percent = health / max_health;
	float mana_percent = mana / max_mana;
	
	HUD_bar->frameW = BAR_FRAME_W;
	sprite_draw(HUD_bar, 0, 0, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y);
	HUD_bar->frameW *= health_percent;
	slog("Health frameW = %f", HUD_bar->frameW);
	sprite_draw(HUD_bar, 0, 1, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y);
	HUD_bar->frameW = BAR_FRAME_W;
	sprite_draw(HUD_bar, 0, 0, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y + HUD_bar->frameH);
	HUD_bar->frameW *= mana_percent;
	sprite_draw(HUD_bar, 0, 2, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y + HUD_bar->frameH);

	sprite_draw(HUD_menu,0,0, graphics_get_renderer(), camera.x + HUD_MENU_DRAW_X, camera.y + HUD_MENU_DRAW_Y);
	if(hud_state_curr == main_menu)
	{
		sprite_draw(HUD_item_bag, 0, 0, graphics_get_renderer(), camera.x + HUD_ITEM_BAG_DRAW_X, camera.y + HUD_ITEM_BAG_DRAW_Y);
	}
}
 
void set_hud_state(HUD_state state)
{
	hud_state_curr = state;
}