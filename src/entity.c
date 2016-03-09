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
			 entityList[i].maxhealth = health;
			 entityList[i].stamina = stamina;
			 entityList[i].state = state;
			 return &entityList[i];
			 break;
		 }
	}
	return NULL;
}

//code based on post by MikeyPro on sdl forum
//link: https://forums.libsdl.org/viewtopic.php?t=11040&sid=0c796abe9954dba84ee2814c32c85f5c

void draw_health_bar(entity *self){
	//percent health to draw
	int barW = self->maxhealth;
	int barH = 15;
	int pw;
	int px;
	float percent = (float)self->health / (float)self->maxhealth; 
	//rects to use as health bars
	SDL_Rect fg_rect;
	SDL_Rect bg_rect = {self->position.x + self->sprite->frameW/2 - barW/2, 
		self->boundBox.y + self->position.y-barH, barW, barH};
	//teal foreground for health
	SDL_Color old; 
	SDL_Color FGColor = {0, 255, 255, 255};
	SDL_Color BGColor = {255, 0, 0, 255};
	
	percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
	pw = (int)((float)bg_rect.w * percent); 
    px = bg_rect.x;

	SDL_GetRenderDrawColor(graphics_get_renderer(), &old.r, &old.g, &old.g, &old.a); 
	if(SDL_SetRenderDrawColor(graphics_get_renderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a))
	 {
		slog("Failed to set render draw color");
     }
    if(SDL_RenderFillRect(graphics_get_renderer(), &bg_rect))slog("Failed to Fill bg rect");
    if(SDL_SetRenderDrawColor(graphics_get_renderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a))
	{
		slog("Failed to set render draw color");
	}
 
    fg_rect = bg_rect;
	fg_rect.x = px;
	fg_rect.w = pw;

    if(SDL_RenderFillRect(graphics_get_renderer(), &fg_rect))slog("Failed to Fill Rect");
    if(SDL_SetRenderDrawColor(graphics_get_renderer(), 0x00, 0x00, 0x00, 0x00))
	{
		slog("Failed to set render draw color");
	}
}

void entity_draw(entity *ent, int x, int y){
	//SDL Main Camera src rect
	sprite_draw(ent->sprite, ent->frame_horizontal, ent->frame_vertical, __gt_graphics_renderer, ent->position.x, ent->position.y);
	draw_health_bar(ent);
	/*SDL_Rect * src =a NULL;
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
<<<<<<< Updated upstream
=======
		if(entityList[i].weapon){
			entityList[i].weapon->face_dir = entityList[i].face_dir;
			entityList[i].weapon->owner_pos.x = entityList[i].position.x;
			entityList[i].weapon->owner_pos.y = entityList[i].position.y;
		}
	}
}

void entity_death(entity *self){
	//death animation

	//free
	entity_free(self);
}

void weapon_touch(entity * self, entity *other)
{
	if (!self || !other)
	{
		slog("Self || Other is NULL");
>>>>>>> Stashed changes
	}
	//apply damage
	other->health -= self->weapon->damage;
	//check if dead
	if(other->health <= 0)
	{
		slog("Entity health is now: %i", other->health);
		entity_death(other);
	}

}

int entity_collide(entity *a, entity*b)
{
<<<<<<< Updated upstream
	SDL_Rect aB = {a->position.x, a->position.y, a->sprite->frameW, a->sprite->frameH};
	SDL_Rect bB = {b->position.x, b->position.y, b->sprite->frameW, b->sprite->frameH};

	return rect_collide(aB,bB);
=======
	SDL_Rect aB = {a->boundBox.x + a->position.x, a->boundBox.y + a->position.y, a->boundBox.w, a->boundBox.h};
	SDL_Rect bB = {b->boundBox.x + b->position.x, b->boundBox.y + b->position.y, b->boundBox.w, b->boundBox.h};

	//slog("Ent: A X:%i Y: %i\n Ent B X: %i Y: %i", a->position.x, a->position.y, b->position.x, b->position.y);

	return rect_collide(aB,bB);
}

int weapon_collision(entity *self, entity *other, Rect_f bound)
{
	Rect_f aB = {(float)other->position.x + (float)other->boundBox.x, (float)other->position.y + other->boundBox.y, other->boundBox.w, other->boundBox.h};
	Rect_f bB = {(float)self->position.x + bound.x, (float)self->position.y + bound.y, bound.w, bound.h};

	if(!self || !self->weapon || !other){
		slog("NO weap or entity");
		return false;
	}
	slog("Owner Pos: X: %i Y:%i\n Weapon Pos X:%f Y:%f\nEntity: X:%f Y:%f",self->position.x, self->position.y,bB.x, bB.y,aB.x, aB.y);
	slog("Bounds Range X:%f - %f Y:%f - %f", bB.x, bB.x + bB.w, bB.y,  bB.y + bB.h);
	slog("Bound other: X:%f - %f Y:%f - %f", aB.x, aB.x + aB.w, aB.y,  aB.y + aB.h);

	if(rect_collide(bB, aB)){
		slog("Weapon has collided");
		weapon_touch(self, other);
		return true;
	}
	return false;
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
							weapon_collision(curr, next, curr->weapon->boundUp);
							break;
					case DOWN:
							weapon_collision(curr, next, curr->weapon->boundDown);
							break;
					case LEFT:
							weapon_collision(curr, next, curr->weapon->boundLeft);
							break;
					case RIGHT:
							weapon_collision(curr, next, curr->weapon->boundRight);
							break;
					default:
						break;
				}
			}
		}
	}
>>>>>>> Stashed changes
}