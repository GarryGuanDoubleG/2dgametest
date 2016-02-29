#include "entity.h"

const int ENTITY_MAX = 2000; // max entities allocated into memory
int entity_count = 0;
entity *entityList; // handle on all entities


void entity_initialize_system(){
	int i;

	entityList = (entity *)malloc(sizeof(entity) * ENTITY_MAX);
	memset(entityList, 0, sizeof(entity)* ENTITY_MAX);

	for(i = 0; i < ENTITY_MAX; i++){
		entityList[i].sprite = NULL;
		entityList[i].inuse = false;
	}
	atexit(entity_close);

}

entity * entity_new(){
	int i = 0;
	for(i = 0; i < ENTITY_MAX; i++){
		if(entityList[i].inuse == true){
			continue;
		}
		memset(&entityList[i], 0, sizeof(entity));
		entityList[i].inuse = true;
		return (&entityList[i]);
	}
	return NULL;
}

entity* entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state){
	int i;
	for(i = 0; i < ENTITY_MAX; i++){
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

	sprite_draw(ent->sprite, ent->frame_horizontal, ent->frame_vertical, __gt_graphics_renderer, ent->position.x, ent->position.y);

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
	for(i = 0; i < ENTITY_MAX; i++){
		 if(entityList[i].inuse == true){
			 entity_free(&entityList[i]);	
		 }
	}
}

void entity_think_all(){
	int i = 0;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].think){
			continue;
		}
		entityList[i].think(&entityList[i]);
	}
}

void entity_update_all(){
	int i = 0;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].update){
			continue;
		}
		entityList[i].update(&entityList[i]);
	}
}

int entity_collide(entity *a, entity*b)
{
	SDL_Rect aB = {a->position.x, a->position.y, a->sprite->frameW, a->sprite->frameH};
	SDL_Rect bB = {b->position.x, b->position.y, b->sprite->frameW, b->sprite->frameH};

	return rect_collide(aB,bB);
}

int weapon_collision(SDL_Rect weap, entity *a{
}

void entity_check_collision_all(){

	int i = 0;
	entity *curr, *next;
	int found_next;

	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].update){
			continue;
		}
		if(!curr){
			curr = &entityList[i];
		}
		else if(found_next){
			next = &entityList[i];
		}
		
		entity_collide(curr, next);
}