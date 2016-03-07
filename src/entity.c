#include "entity.h"
#include "simple_logger.h"
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
		if(entityList[i].weapon){
			entityList[i].weapon->face_dir = entityList[i].face_dir;
			entityList[i].weapon->owner_pos.x = entityList[i].position.x;
			entityList[i].weapon->owner_pos.y = entityList[i].position.y;
		}

	}
}

int entity_collide(entity *a, entity*b)
{
	SDL_Rect aB = {a->boundBox.x + a->position.x, a->boundBox.y + a->position.y, a->boundBox.w, a->boundBox.h};
	SDL_Rect bB = {b->boundBox.x + b->position.x, b->boundBox.y + b->position.y, b->boundBox.w, b->boundBox.h};

	//slog("Ent: A X:%i Y: %i\n Ent B X: %i Y: %i", a->position.x, a->position.y, b->position.x, b->position.y);

	return rect_collide(aB,bB);
}

int weapon_collision(Weapon *weap, entity *a, SDL_Rect bound)
{
	SDL_Rect aB = {a->position.x, a->position.y, a->sprite->frameW, a->sprite->frameH};
	SDL_Rect bB = {weap->owner_pos.x + bound.x, weap->owner_pos.x + bound.y, bound.h, bound.w};

	slog("Wep Coll");
	if(!weap || !a){
		slog("NO weap or entity");
		return false;
	}

	slog("Weapon Pos X:%i Y:%i", bB.x, bB.y);
	int collide = rect_collide(bB, aB);
	
	if(collide){
		slog("Collision");
	}
	
	return collide;
}

void entity_check_collision_all(){

	int i = 0;
	int j = 0;
	entity *curr = NULL;
	entity *next = NULL;
	int found_next;

	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].update){
			continue;
		}
		curr = &entityList[i];
		for(j = 0; j < ENTITY_MAX; j++){
			if(!entityList[j].inuse){
				continue;
			}
			if(!entityList[j].update){
				continue;
			}
			if(curr == &entityList[j]){
				continue;
			}
			if(!curr){
				slog("Curr is null");
				continue;
			}
			next = &entityList[j];
			if((!next || !curr) && curr == next )
			{
				continue;
			}
			if(entity_collide(curr, next))
			{
				/*slog("Ent: A X:%i Y: %i\n Ent B X: %i Y: %i", 
					curr->position.x, curr->position.y, next->position.x, next->position.y);
				slog("Collision!");*/
			}
			if(curr->weapon && curr->weapon->active){
				switch(curr->weapon->face_dir){
					case UP:
							weapon_collision(curr->weapon, next, curr->weapon->boundUp);
							break;
					case DOWN:
							weapon_collision(curr->weapon, next, curr->weapon->boundDown);
							break;
					case LEFT:
							weapon_collision(curr->weapon, next, curr->weapon->boundLeft);
							break;
					case RIGHT:
							weapon_collision(curr->weapon, next, curr->weapon->boundRight);
							break;
					default:
						break;
				}
			}
		}
	}
}