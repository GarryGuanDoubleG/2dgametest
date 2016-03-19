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
			 entityList[i].maxhealth = health;
			 entityList[i].stamina = stamina;
			 entityList[i].state = state;
			 entityList[i].player = false; //set to true on player init
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
	SDL_Rect bg_rect = {self->position.x + self->sprite->frameW/2 - barW/2, 
	self->boundBox.y + self->position.y-barH, barW, barH};
	SDL_Rect fg_rect;
	//teal foreground for health
	SDL_Color old; 
	SDL_Color FGColor = {0, 255, 255, 255};
	SDL_Color BGColor = {255, 0, 0, 255};
	

	SDL_GetRenderDrawColor(graphics_get_renderer(), &old.r, &old.g, &old.g, &old.a); 
	if(SDL_SetRenderDrawColor(graphics_get_renderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a))
	 {
		slog("Failed to set render draw color");
     }
	if(!rect_collide(bg_rect, graphics_get_player_cam()))
	{
		SDL_SetRenderDrawColor(graphics_get_renderer(), 0x00, 0x00, 0x00, 0x00);
		return;
	}
		bg_rect.x -= graphics_get_player_cam().x;
		bg_rect.y -= graphics_get_player_cam().y;
		fg_rect = bg_rect;
		px = bg_rect.x;
		percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
		pw = (int)((float)bg_rect.w * percent); 

		if(SDL_RenderFillRect(graphics_get_renderer(), &bg_rect))
		{
			slog("Failed to Fill bg rect");
		}
    if(SDL_SetRenderDrawColor(graphics_get_renderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a))
	{
		slog("Failed to set render draw color");
	}
 
	fg_rect.x = px;
	fg_rect.w = pw;
	if(SDL_RenderFillRect(graphics_get_renderer(), &fg_rect))
	{
		slog("Failed to Fill Rect");
	}

    if(SDL_SetRenderDrawColor(graphics_get_renderer(), 0x00, 0x00, 0x00, 0x00))
	{
		slog("Failed to set render draw color");
	}
}
//end

void entity_draw(entity *ent, int x, int y){
	//SDL Main Camera src rect
	SDL_Rect entRect = {ent->position.x + ent->boundBox.x, ent->position.y + ent->boundBox.h, ent->boundBox.w, ent->boundBox.h};
	sprite_draw(ent->sprite, ent->frame_horizontal, ent->frame_vertical, __gt_graphics_renderer, ent->position.x, ent->position.y);
	draw_health_bar(ent);
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
	SDL_Rect aB = {a->boundBox.x + a->position.x, a->boundBox.y + a->position.y, a->boundBox.w, a->boundBox.h};
	SDL_Rect bB = {b->boundBox.x + b->position.x, b->boundBox.y + b->position.y, b->boundBox.w, b->boundBox.h};

	//slog("Ent: A X:%i Y: %i\n Ent B X: %i Y: %i", a->position.x, a->position.y, b->position.x, b->position.y);

	return rect_collide(aB,bB);
}

void weapon_collide_draw_box(Rect_f self, Rect_f other)
{
 
	//rects to use as health bars
	SDL_Rect bg_rect = {self.x,self.y,self.w,self.h};
	SDL_Rect fg_rect = {other.x,other.y,other.w,other.h};
	//teal foreground for health
	SDL_Color old; 
	SDL_Color FGColor = {0, 255, 255, 255}; // other
	SDL_Color BGColor = {255, 0, 0, 255}; //weapon

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
 
    if(SDL_RenderFillRect(graphics_get_renderer(), &fg_rect))slog("Failed to Fill Rect");
    if(SDL_SetRenderDrawColor(graphics_get_renderer(), 0x00, 0x00, 0x00, 0x00))
	{
		slog("Failed to set render draw color");
	}
}
int weapon_collision(entity *self, entity *other, Rect_f bound)
{
	Rect_f aB = {(float)other->position.x + (float)other->boundBox.x, (float)other->position.y + other->boundBox.y, other->boundBox.w, other->boundBox.h};
	Rect_f bB = {(float)self->position.x + bound.x + self->weapon->boundOffset.x, (float)self->position.y + bound.y + self->weapon->boundOffset.y, bound.w, bound.h};

	if(!self || !self->weapon || !other){
		slog("NO weap or entity");
		return false;
	}

	slog("wep bound is X:%f Y:%f W:%f H:%f",bound.x,bound.y,bound.w,bound.h);
	/*slog("Owner Pos: X: %i Y:%i\n Weapon Pos X:%f Y:%f\nEntity: X:%f Y:%f",self->position.x, self->position.y,bB.x, bB.y,aB.x, aB.y);
	slog("Bounds Range X:%f - %f Y:%f - %f", bB.x, bB.x + bB.w, bB.y,  bB.y + bB.h);
	slog("Bound other: X:%f - %f Y:%f - %f", aB.x, aB.x + aB.w, aB.y,  aB.y + aB.h);*/

	if(rect_collide(bB, aB)){
		slog("Weapon has collided");
		weapon_touch(self, other);
	// weapon_collide_draw_box(bB,aB); used for visual collision box
		return true;
	}
	return false;
}

void weapon_collision(entity *owner)
{
	int i;

	if(!owner || !owner->weapon)
	{
		slog("Entity or Weapon is NULL. No Collision");
	}
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].update){
			continue;
		}
		if(owner == &entityList[i]){
			continue;
		}
		if(owner->weapon && owner->weapon->active){
			switch(owner->weapon->face_dir){
				case UP:
						weapon_collision(owner, &entityList[i], owner->weapon->boundUp);
						break;
				case DOWN:
						weapon_collision(owner, &entityList[i], owner->weapon->boundDown);
						break;
				case LEFT:
						weapon_collision(owner, &entityList[i], owner->weapon->boundLeft);
						break;
				case RIGHT:
						weapon_collision(owner, &entityList[i], owner->weapon->boundRight);
						break;
				default:
					break;
			}
		}
	}
}
void entity_check_collision_all()
{
	int i = 0;
	int j = 0;
	entity *curr = NULL;
	entity *next = NULL;

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
			entity_collide(curr, next);			
		}
	}
}

//player 
entity * entity_get_player()
{
	int i;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].update){
			continue;
		}
		if(!entityList[i].player)
		{
			continue;
		}
		return &entityList[i];
	}
}