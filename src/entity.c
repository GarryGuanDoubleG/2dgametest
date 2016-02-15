#include "entity.h"

const int MAX_ENTITY = 2000; // max entities allocated into memory
int entity_count = 0;
entity *entityList; // handle on all entities


void entity_initialize_system(){
	int i;

	entityList = (entity *)malloc(sizeof(entity) * MAX_ENTITY);
	memset(entityList, 0, sizeof(entity)* MAX_ENTITY);

	for(i = 0; i < MAX_ENTITY; i++){
		entityList[i].sprite = NULL;
		entityList[i].inuse = false;
	}
	atexit(entity_close);

}
entity* entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state){
	int i;
	for(i = 0; i < MAX_ENTITY; i++){
		 if(entityList[i].inuse == false){
			 entity_count++;
			 entityList[i].inuse = true;
			 entityList[i].sprite = sprite;
			 entityList[i].position = pos;
			 entityList[i].health = health;
			 entityList[i].stamina = stamina;
			 entityList[i].state = state;
			 return &entityList[i];
			 break;
		 }
	}
	return NULL;
}

void entity_draw(entity *ent, int x, int y){
	//SDL Main Camera src rect

	sprite_draw(ent->sprite, ent->frame_horizontal, ent->frame_vertical, __gt_graphics_renderer, x, y);

	/*SDL_Rect * src = NULL;
	SDL_Rect dest = {x, y, ent->sprite->imageW, ent->sprite->imageH};

	SDL_RenderCopy(__gt_graphics_renderer, ent->sprite->image, NULL, &dest);//replace null with main camera
	*/
}

void entity_free(entity* ent){ //makes entity->inuse false so new ent can be initialized in this mem location
	ent->inuse = false;
	ent->sprite = NULL;
	entity_count--;
}

void entity_close(){ //deallocates all entities
	int i;
	for(i = 0; i < MAX_ENTITY; i++){
		 if(entityList[i].inuse == true){
			 entity_free(&entityList[i]);	
		 }
	}
}
