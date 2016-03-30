#ifndef _INVENTORY_H_
#define _INVENTORY_H_
#include "weapon.h"
#include "armor.h"
#include "items.h"
/**
* @brief maximum number of items that can be stored in inventory
*/
extern const int ITEMS_MAX;
/**
* @brief maximum number of armor equipment that can be stored in inventory
*/
extern const int ARMORS_MAX;
/**
* @brief maximum number of weapons that can be stored in inventory
*/
extern const int WEAPONS_MAX;

/**
* @brief pointer to list of items currently in inventory
*/
extern item *inv_items;
/**
* @brief pointer to current weapons in inventory
*/
extern Weapon *inv_weapons;
/**
* @brief pointer to current armors in inventory
*/
extern Armor *inv_armors;
//Spells *inv_spells[]
//items that can be displayed in the hud
/**
* @brief number of items that can be stored in each item bag
*/
extern const int inv_bag_size;
/**
* @brief allocates memory for items, weapons, and armor inventories
*/
void inventory_init();
/**
* @brief adds item by name into inventory
* @param uses cstring to loop through itemlist and find matching item to add to inventory
*/
void inventory_add(char* name);

#endif