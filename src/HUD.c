#include "HUD.h"

Sprite * HUD_bar;
Sprite * HUD_menu;
Sprite * HUD_item_bag;
Sprite * HUD_axe;

static HUD_state hud_state_curr;

void hud_init()
{
	hud_state_curr = main_menu;

	HUD_bar = Sprite_Load(HEALTH_MANA_BAR_PATH, BAR_IMG_W, BAR_IMG_H, BAR_FRAME_W, BAR_FRAME_H);
	HUD_bar->fpl = 1;
	HUD_menu = Sprite_Load(HUD_MENU_GRID_PATH, HUD_MENU_IMG_W, HUD_MENU_IMG_H, HUD_MENU_FRAME_W, HUD_MENU_FRAME_H);
	HUD_item_bag = Sprite_Load(HUD_ITEM_BAG_PATH, HUD_ITEM_BAG_IMG_W, HUD_ITEM_BAG_IMG_H, HUD_ITEM_BAG_FRAME_W, HUD_ITEM_BAG_FRAME_H);
	HUD_axe = Sprite_Load(HUD_AXE_PATH, HUD_AXE_IMG_W, HUD_AXE_IMG_H, HUD_AXE_FRAME_W, HUD_AXE_FRAME_H);
}

void hud_draw(SDL_Rect camera, float health, float max_health, float mana, float max_mana)
{
	Sprite sprite_bar;

	float health_percent = health / max_health;
	float mana_percent;

	float bg_bar_width;
	float health_bar_width;
	float mana_bar_width;

	int frame;
	int i;

	Vec2d draw_pos_h_bar;
	Vec2d draw_pos_m_bar;
	Vec2d cam_pos;
	Vec2d draw_pos;

	sprite_bar = *HUD_bar;

	health_percent = health / max_health;
	mana_percent = mana / max_mana;

	bg_bar_width = BAR_FRAME_W;
	health_bar_width = bg_bar_width * health_percent;
	mana_bar_width = bg_bar_width * mana_percent;

	frame = 0;

	Vec2dSet(cam_pos, camera.x, camera.y);
	draw_pos_h_bar.x = cam_pos.x + HUD_BAR_DRAW_X;
	draw_pos_h_bar.y = cam_pos.y + HUD_BAR_DRAW_Y;

	draw_pos_m_bar = draw_pos_h_bar;
	draw_pos_m_bar.y += HUD_bar->frameH;
	
	//draw bar background
	sprite_bar.frameW = bg_bar_width;
	//health bar bg
	Sprite_Draw(&sprite_bar, 0, Graphics_Get_Renderer(), draw_pos_h_bar);
	//mana bar bg
	Sprite_Draw(&sprite_bar, 0, Graphics_Get_Renderer(), draw_pos_m_bar);

	//health
	sprite_bar.frameW = BAR_FRAME_W * health_percent;
	Sprite_Draw(&sprite_bar, 1, Graphics_Get_Renderer(), draw_pos_h_bar);
	
	//mana
	sprite_bar.frameW = BAR_FRAME_W * mana_percent;
	Sprite_Draw(&sprite_bar, 2, Graphics_Get_Renderer(), draw_pos_m_bar);
	
	//menu

	Vec2dSet(draw_pos, camera.x + HUD_MENU_DRAW_X, camera.y + HUD_MENU_DRAW_Y);
	Sprite_Draw(HUD_menu,0, Graphics_Get_Renderer(), draw_pos);

	if(hud_state_curr == main_menu)
	{
		Vec2dSet(draw_pos, camera.x + HUD_ITEM_BAG_DRAW_X, camera.y + HUD_ITEM_BAG_DRAW_Y);
		Sprite_Draw(HUD_item_bag, 0, Graphics_Get_Renderer(), draw_pos);

		Vec2dSet(draw_pos, camera.x + HUD_AXE_DRAW_X, camera.y + HUD_AXE_DRAW_Y);
		Sprite_Draw(HUD_axe, 0,  Graphics_Get_Renderer(), draw_pos);
	}

	if(hud_state_curr == inventory1)
	{
		for(i = 0; i < inv_bag_size; i++)		
		{
			int draw_x = HUD_GRID_OFFSET_X + HUD_MENU_DRAW_X + camera.x;
			int draw_y = HUD_GRID_OFFSET_Y + HUD_MENU_DRAW_Y + camera.y;
			Vec2d draw_pos;

			if(!inv_items[i].inuse)
			{
				continue;
			}

			draw_x += HUD_GRIDBOX_W * (i % HUD_MENU_COL); 
			draw_y += HUD_GRIDBOX_H * (i / HUD_MENU_COL); 

			Vec2dSet(draw_pos, draw_x, draw_y);

			Sprite_Draw(inv_items[i].icon, 0, Graphics_Get_Renderer(), draw_pos);
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