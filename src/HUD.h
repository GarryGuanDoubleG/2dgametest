#ifndef _HUD_H_
#define _HUD_H_
#include "sprite.h"
#include "simple_logger.h"
#include "inventory.h"

/**
* @brief pointer to sprite of player's health bar that HUD updates based on current health
*/
extern Sprite2 * health_bar;
/**
* @brief pointer to sprite of player's mana bar that HUD updates based on current mana
*/
extern Sprite2 * mana_bar;
/**
* @brief pointer to sprite of background of health and mana bars
*/
extern Sprite2 * bg_bar;

/**
* @brief states of the HUD to determine what to display in the menu HUD element
*/
enum HUD_state
{
	main_menu,
	inventory1,
	inventory2,
	spell,
	equip,
	build1,
	build2,
};
/*
* @brief loads the sprites for the hud into memory and stores a handle on them
*/
void hud_init();
/**
* @brief renders the hud on the screen relative to the camera position
* @param camera position to use for drawing relative location. Health & max health to determine the fill length of health bar. Same with mana
*/
void hud_draw(SDL_Rect camera, float health, float max_health, float mana, float max_mana);
/**
* @brief changes the state of the hud and returns whether hud was already in the state
* @param state to set HUD's state
* @return returns 1 or 0 based on whether HUD was already in the parameter state
*/
int set_hud_state(HUD_state state);
/**
* @brief define terms for all HUD elements including filepath, image Width and height, and frame width and height
* of health & mana bar, in game menu and icons
*/
#define HEALTH_MANA_BAR_PATH "images/HUD/SleekBars.png"
#define BAR_IMG_W 128 //original size of image
#define BAR_IMG_H 32
#define BAR_FRAME_H 32
#define BAR_FRAME_W 360
//individual bars
#define BAR_HEALTH_IMG_H 128
#define BAR_MANA_IMG_H 32
#define BAR_BG_IMG_H 0

#define HUD_BAR_DRAW_X 10
#define HUD_BAR_DRAW_Y SCREEN_HEIGHT*.85f

#define HUD_MENU_GRID_PATH "images/HUD/HUD_grid.png"
#define HUD_MENU_IMG_W 407
#define HUD_MENU_IMG_H 313
#define HUD_MENU_FRAME_W 400
#define HUD_MENU_FRAME_H 300
#define HUD_MENU_DRAW_X SCREEN_WIDTH - HUD_MENU_FRAME_W
#define HUD_MENU_DRAW_Y (SCREEN_HEIGHT - HUD_MENU_FRAME_H)
#define HUD_MENU_COL 4
#define HUD_MENU_ROW 3

#define HUD_GRID_OFFSET_X 35
#define HUD_GRID_OFFSET_Y 45
#define HUD_GRIDBOX_W 87
#define HUD_GRIDBOX_H 84

#define HUD_ITEM_BAG_PATH "images/HUD/HUD_item_bag_icon.png"
#define HUD_ITEM_BAG_IMG_W 56
#define HUD_ITEM_BAG_IMG_H 56
#define HUD_ITEM_BAG_FRAME_W 85
#define HUD_ITEM_BAG_FRAME_H 75
#define HUD_ITEM_BAG_DRAW_X HUD_MENU_DRAW_X + HUD_GRID_OFFSET_X
#define HUD_ITEM_BAG_DRAW_Y HUD_MENU_DRAW_Y + HUD_GRID_OFFSET_Y

#define HUD_AXE_PATH "images/HUD/axe_1.png"
#define HUD_AXE_IMG_W 90
#define HUD_AXE_IMG_H 74
#define HUD_AXE_FRAME_W 85
#define HUD_AXE_FRAME_H 69
#define HUD_AXE_DRAW_X  HUD_MENU_DRAW_X + HUD_GRID_OFFSET_X + HUD_GRIDBOX_W  
#define HUD_AXE_DRAW_Y  HUD_MENU_DRAW_Y + HUD_GRID_OFFSET_Y + HUD_GRIDBOX_H  

#endif