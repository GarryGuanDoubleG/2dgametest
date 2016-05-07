#include "HUD.h"

Sprite * HUD_bar;
Sprite * HUD_menu;
Sprite * HUD_item_bag;
Sprite * HUD_axe;

static HUD_state hud_state_curr;

void hud_init()
{
	hud_state_curr = main_menu;

	HUD_bar = sprite_load(HEALTH_MANA_BAR_PATH, BAR_IMG_W, BAR_IMG_H, BAR_FRAME_W, BAR_FRAME_H);
	HUD_menu = sprite_load(HUD_MENU_GRID_PATH, HUD_MENU_IMG_W, HUD_MENU_IMG_H, HUD_MENU_FRAME_W, HUD_MENU_FRAME_H);
	HUD_item_bag = sprite_load(HUD_ITEM_BAG_PATH, HUD_ITEM_BAG_IMG_W, HUD_ITEM_BAG_IMG_H, HUD_ITEM_BAG_FRAME_W, HUD_ITEM_BAG_FRAME_H);
	HUD_axe = sprite_load(HUD_AXE_PATH, HUD_AXE_IMG_W, HUD_AXE_IMG_H, HUD_AXE_FRAME_W, HUD_AXE_FRAME_H);
}

void hud_draw(SDL_Rect camera, float health, float max_health, float mana, float max_mana)
{

	float health_percent = health / max_health;
	float mana_percent = mana / max_mana;

	int i;
	
	HUD_bar->frameW = BAR_FRAME_W;
	sprite_draw(HUD_bar, 0, 0, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y);
	HUD_bar->frameW *= health_percent;
	sprite_draw(HUD_bar, 0, 1, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y);
	HUD_bar->frameW = BAR_FRAME_W;
	sprite_draw(HUD_bar, 0, 0, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y + HUD_bar->frameH);
	HUD_bar->frameW *= mana_percent;
	sprite_draw(HUD_bar, 0, 2, graphics_get_renderer(), camera.x + HUD_BAR_DRAW_X, camera.y + HUD_BAR_DRAW_Y + HUD_bar->frameH);

	sprite_draw(HUD_menu,0,0, graphics_get_renderer(), camera.x + HUD_MENU_DRAW_X, camera.y + HUD_MENU_DRAW_Y);

	if(hud_state_curr == main_menu)
	{
		sprite_draw(HUD_item_bag, 0, 0, graphics_get_renderer(), camera.x + HUD_ITEM_BAG_DRAW_X, camera.y + HUD_ITEM_BAG_DRAW_Y);
		sprite_draw(HUD_axe, 0, 0, graphics_get_renderer(), camera.x + HUD_AXE_DRAW_X, camera.y + HUD_AXE_DRAW_Y);
	}
	if(hud_state_curr == inventory1)
	{
		for(i = 0; i < inv_bag_size; i++)		
		{
			int draw_x = HUD_GRID_OFFSET_X + HUD_MENU_DRAW_X + camera.x;
			int draw_y = HUD_GRID_OFFSET_Y + HUD_MENU_DRAW_Y + camera.y;

			if(!inv_items[i].inuse)
			{
				continue;
			}

			draw_x += HUD_GRIDBOX_W * (i % HUD_MENU_COL); 
			draw_y += HUD_GRIDBOX_H * (i / HUD_MENU_COL); 

			sprite_draw(inv_items[i].icon, 0, 0, graphics_get_renderer(), draw_x, draw_y);
		}
		
	}

}
 
int set_hud_state(HUD_state state)
{
	if(hud_state_curr == state)
	{
		hud_state_curr = HUD_state::main_menu;
		return false;
	}
	else
	{
		hud_state_curr = state;
		return true;
	}
}