#include "inventory.h"

const int ITEMS_MAX = 1000;
const int ARMORS_MAX = 1000;
const int WEAPONS_MAX = 1000;
const int inv_bag_size = 12;

item *inv_items;
Weapon *inv_weapons;
Armor *inv_armors;

void inventory_close()
{
	int i;

	for(i = 0; i < ITEMS_MAX; i++){
		inv_items[i].inuse = false;
		inv_items[i].icon = NULL;
		inv_items[i].item_model = NULL;
	}
	memset(inv_items, 0, sizeof(item)* ITEMS_MAX);
	free(inv_items);

	memset(inv_weapons, 0, sizeof(Weapon)* WEAPONS_MAX);
	free(inv_weapons);
	memset(inv_armors, 0, sizeof(Armor)* ARMORS_MAX);
	free(inv_armors);

}

void inventory_init()
{
	int i;

	inv_items = (item*)malloc(sizeof(item) * ITEMS_MAX);
	if(inv_items)memset(inv_items, 0, sizeof(item)* ITEMS_MAX);

	inv_weapons = (Weapon*)malloc(sizeof(Weapon) * WEAPONS_MAX);
	memset(inv_weapons, 0, sizeof(Weapon)* WEAPONS_MAX);
	
	inv_armors = (Armor*)malloc(sizeof(Armor) * ARMORS_MAX);
	memset(inv_armors, 0, sizeof(Armor)* ARMORS_MAX);

	for(i = 0; i < ITEMS_MAX; i++){
		inv_items[i].inuse = false;
	}
	
	for(i = 0; i < WEAPONS_MAX; i++){
		inv_weapons[i].inuse = false;
	}

	for(i = 0; i < ITEMS_MAX; i++){
		inv_armors[i].inuse = false;
	}

	atexit(inventory_close);
}

void slog_inv()
{
	int i;
	for( i = 0; i < ITEMS_MAX; i++)
	{
		if(inv_items[i].inuse)
		{
			slog("Inv slot: %i Item: %s", i, inv_items[i].name);
		}
	}
}

void inventory_add(char* name)
{
	int i;
	
	if(!name)
	{
		slog("Name is NULL");
	}

	for( i = 0; i < ITEMS_MAX; i++)
	{
		if(inv_items[i].inuse)
		{
			continue;
		}
		inv_items[i] = *(getItem(name));
		inv_items[i].inuse = true;
		break;
	}

	slog_inv();
}
