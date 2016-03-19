#include "inventory.h"

const int ITEMS_MAX = 1000;

void inventory_close()
{
	int i;

	for(i = 0; i < ITEMS_MAX; i++){
		inv_items[i].inuse = false;
		inv_items[i].icon = NULL;
		inv_items[i].item_model = NULL;
	}
}

void inventory_init()
{
	int i;

	inv_items = (item*)malloc(sizeof(item) * ITEMS_MAX);
	memset(inv_items, 0, sizeof(item)* ITEMS_MAX);

	for(i = 0; i < ITEMS_MAX; i++){
		inv_items[i].inuse = false;
	}

	atexit(inventory_close);
}

