#ifndef _INVENTORY_H_
#define _INVENTORY_H_
#include "weapon.h"
#include "armor.h"
#include "items.h"

extern const int ITEMS_MAX;
extern const int ARMORS_MAX;
extern const int WEAPONS_MAX;

extern item *inv_items;
extern Weapon *inv_weapons;
extern Armor *inv_armors;
//Spells *inv_spells[]
//items that can be displayed in the hud
extern const int inv_bag_size;

void inventory_init();
void inventory_add(char* name);

#endif