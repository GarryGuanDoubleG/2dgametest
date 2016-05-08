#include "entity.h"
#include "simple_logger.h"
#include "load.h"

const int ENTITY_MAX = 2000; // max entities allocated into memory
int entity_count = 0;
entity *entityList; // handle on all entities
Dict * entity_defs = NULL;// dictionary of all entity definitions

extern entity *player;

void entity_initialize_system(){
	int i;
	Line key;
	Dict * temp;
	entityList = (entity *)malloc(sizeof(entity) * ENTITY_MAX);
	memset(entityList, 0, sizeof(entity)* ENTITY_MAX);

	for(i = 0; i < ENTITY_MAX; i++){
		entityList[i].sprite = NULL;
		entityList[i].inuse = false;
	}
	slog("LOADING ENTITY DEF");
	entity_defs = load_dict_from_file("def/test.def");
	if(!entity_defs)
	{
		slog("Error, could not load entity def");
	}
	else if(entity_defs->data_type != DICT_HASH)
	{
		slog("Def file is not a hash");
	}
	else
	{
		for(i = 0; i < entity_defs->item_count; i++)
		{
			temp = dict_get_hash_nth(key, entity_defs, i);
			slog("%i: \n%s \n%s", i, key, (const char*)temp->keyValue);
		}
	}
	slog("FINISHED LOADING");
	atexit(entity_close);

}

entity * entity_new(){
	int i = 0;
	for(i = 0; i < ENTITY_MAX; i++){
		if(entityList[i].inuse == true){
			continue;
		}
//		path_free(entityList[i].path);
		memset(&entityList[i], 0, sizeof(entity));
		entityList[i].inuse = true;
		return (&entityList[i]);
	}
	return NULL;
}
//loads structure
entity* struct_load(Sprite *sprite, int health, int defense, int type)
{
	int i;
	for(i = 0; i < entity_count + 1; i++)
	{
		 if(entityList[i].inuse == false)
		 {
			 entity_count++;
			 entityList[i].inuse = true;
			 entityList[i].sprite = sprite;
			 entityList[i].health = health;
			 entityList[i].maxhealth = health;			 
			 entityList[i].structure = true;
			 entityList[i].placed = false;
			 entityList[i].selected = false;
			 
			 return &entityList[i];
			 break;
		 }
	}
	return NULL;
}

entity* entity_load(Sprite *sprite,Vec2d pos, int health, int stamina, int state){
	int i;
	for(i = 0; i < entity_count + 1; i++)
	{
		 if(entityList[i].inuse == false)
		 {
			 entity_count++;
			 entityList[i].inuse = true;
			 entityList[i].sprite = sprite;
			 entityList[i].position = pos;
			 entityList[i].health = health;
			 entityList[i].maxhealth = health;
			 entityList[i].stamina = stamina;
			 entityList[i].state = state;
			 entityList[i].followPath = ent_follow_path;
			 entityList[i].path = NULL;
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
	

	SDL_GetRenderDrawColor(Graphics_Get_Renderer(), &old.r, &old.g, &old.g, &old.a); 
	if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a))
	 {
		slog("Failed to set render draw color");
     }
	if(!rect_collide(bg_rect, Graphics_Get_Player_Cam()))
	{
		SDL_SetRenderDrawColor(Graphics_Get_Renderer(), 0x00, 0x00, 0x00, 0x00);
		return;
	}
		bg_rect.x -= Graphics_Get_Player_Cam().x;
		bg_rect.y -= Graphics_Get_Player_Cam().y;
		fg_rect = bg_rect;
		px = bg_rect.x;
		percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
		pw = (int)((float)bg_rect.w * percent); 

		if(SDL_RenderFillRect(Graphics_Get_Renderer(), &bg_rect))
		{
			slog("Failed to Fill bg rect");
		}
    if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a))
	{
		slog("Failed to set render draw color");
	}
 
	fg_rect.x = px;
	fg_rect.w = pw;
	if(SDL_RenderFillRect(Graphics_Get_Renderer(), &fg_rect))
	{
		slog("Failed to Fill Rect");
	}

    if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), 0x00, 0x00, 0x00, 0x00))
	{
		slog("Failed to set render draw color");
	}
}
//end

void entity_draw(entity *ent){
	//SDL Main Camera src rect	
	Sprite_Draw(ent->sprite, ent->frame, Graphics_Get_Renderer(), ent->position);
	if(ent == player)
	{
		draw_health_bar(ent);
	}
}

void entity_free(entity* ent){ //makes entity->inuse false so new ent can be initialized in this mem location
	ent->inuse = false;
	ent->sprite = NULL;
	path_free(ent->path);
	entity_count--;

}

void entity_close(){ //deallocates all entities
	int i;
	for(i = 0; i < ENTITY_MAX; i++){
		 if(entityList[i].inuse == true){
			 entity_free(&entityList[i]);	
		 }
	}
	memset(entityList, 0, sizeof(entity) * ENTITY_MAX);
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
		/*if(entityList[i].path)
		{
			path_free(entityList[i].path);
		}*/
		entityList[i].think(&entityList[i]);
	}
}

void entity_update_all(){
	int i = 0;
	Vec2d new_pos;
	Vec2d old_pos;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(!entityList[i].sprite)
		{
			entity_free(&entityList[i]);
		}
		//check for map bounds
		if(entityList[i].position.x < 0 ) 
		{
			entityList[i].position.x =0;
		}
		if(entityList[i].position.x + entityList[i].sprite->frameW > TILE_ROWS * TILE_WIDTH)
		{
			entityList[i].position.x  = TILE_ROWS * TILE_WIDTH - entityList[i].sprite->frameW;
		}
		if(entityList[i].position.y < 0 ) 
		{
			entityList[i].position.y =0;
		}
		if(entityList[i].position.y + entityList[i].sprite->frameH > TILE_COLUMNS * TILE_HEIGHT)
		{
			entityList[i].position.y  = TILE_COLUMNS * TILE_HEIGHT - entityList[i].sprite->frameH;
		}

		if(!entityList[i].update){
			continue;
		}

		entityList[i].update(&entityList[i]);
		//don't free player
		if(entityList[i].health <= 0 && !(&entityList[i] == player))
		{
			entity_free(&entityList[i]);
		}
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
	if(other->team != self->team)
	{
		other->health -= self->weapon->damage;
	}
	//check if dead
	if(other->health <= 0)
	{
		slog("Entity health is now: %i", other->health);
		entity_death(other);
	}

}
int entity_check_collision(entity * self)
{
	int i;
	for(i = 0; i < entity_count + 1; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(&entityList[i] == self)
		{
			continue;
		}
		if(entity_collide(self, &entityList[i]))
		{
			return true;
		}
	}
	return false;
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

	SDL_GetRenderDrawColor(Graphics_Get_Renderer(), &old.r, &old.g, &old.g, &old.a); 
	if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a))
	 {
		slog("Failed to set render draw color");
     }
    if(SDL_RenderFillRect(Graphics_Get_Renderer(), &bg_rect))slog("Failed to Fill bg rect");
    if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a))
	{
		slog("Failed to set render draw color");
	}
 
    if(SDL_RenderFillRect(Graphics_Get_Renderer(), &fg_rect))slog("Failed to Fill Rect");
    if(SDL_SetRenderDrawColor(Graphics_Get_Renderer(), 0x00, 0x00, 0x00, 0x00))
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
			if((!next || !curr) || curr == next )
			{
				continue;
			}
			if(entity_collide(curr, next))
			{
				if(curr->touch)
				{				
					curr->touch(curr, next);
				}
			}
		}
	}
}

//player 
entity * entity_get_player()
{
	return player;
}

void ent_follow_path(entity *self)
{
	Vec2d tile_pos;
	Vec2d tile_center_pos;
	Vec2d self_center_pos = { ENT_CENTER_X(self), ENT_CENTER_Y(self)};
	Vec2d player_center_pos = { ENT_CENTER_X(entity_get_player()), ENT_CENTER_Y(entity_get_player()) };
	Vec2d new_vel;
	//smaller bounds mean closer
	Rect_f tile_bound = {-1,-1, TILE_WIDTH/3, TILE_HEIGHT/3};
	Rect_f self_bound = {self->position.x + self->boundBox.x, self->position.y + self->boundBox.y,
						 self->boundBox.w, self->boundBox.h};
	float x;
	float y;

	int distance;
	if(!self) 
	{
		return;
	}
	if(self->state != STATE_AGGRO)
	{
		return;
	}
	if(!self->path)
	{
	//	slog("No Path");
		return;
		/*Vec2dSubtract(self_center_pos,player_center_pos,new_vel);
		Normalize2d(new_vel);
		VectorScale(new_vel, new_vel, 10);
		self->velocity = new_vel;
		slog("New Vel X:%f Y:%f", new_vel.x, new_vel.y);
		x = new_vel.x;
		y = new_vel.y;
		while((x * x + y * y) < 25)
		{
			//VectorScale(new_vel, new_vel, 2);
			x *= 2;
			y *= 2;
		}
		new_vel.x = x;
		new_vel.y = y;
		return;*/
	}
	else
	{
		/*slog("Next is: %i", self->path->tile_index);
		slog("Player is %i", tile_get_tile_number(entity_get_player()->position, entity_get_player()->boundBox));*/
		 tile_pos = tile_get_pos(self->path->tile_index);

		 tile_center_pos.x = TILE_CENTER_X(tile_pos);
		 tile_center_pos.y = TILE_CENTER_Y(tile_pos);

		 tile_bound.x = tile_center_pos.x;
		 tile_bound.y = tile_center_pos.y;
	}

/*	if((TILE_CENTER_X(tile_pos) == ENT_CENTER_X(self)) && 
	   TILE_CENTER_Y(tile_pos) == ENT_CENTER_Y(self) )*/
	if(rect_collide(tile_bound, self_bound))
	{
//		slog("rect collide");
		path_free_node(&(self->path));
		self->velocity.x = 0;
		self->velocity.y = 0;
	}
	else
	{	
		Vec2dSubtract(tile_center_pos, self_center_pos, new_vel);
		Normalize2d(new_vel);
		VectorScale(new_vel, new_vel, 5);
		x = new_vel.x;
		y = new_vel.y;
		while((x * x + y * y) < 49)
		{
			//VectorScale(new_vel, new_vel, 2);
			x *= 2;
			y *= 2;
		}
		new_vel.x = x;
		new_vel.y = y;
		self->velocity = new_vel;
	//	slog("New Vel X:%f Y:%f", new_vel.x, new_vel.y);
	}

}

entity * ent_find_nearest_enemy(entity *self)
{
	int i;
	float min_dist = 99999;
	float distance;
	int ent_index = -1;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(entityList[i].team == self->team)
		{
			continue;
		}
		if(&entityList[i] == self)
		{
			continue;
		}

		distance = Vec2dDistance(entityList[i].position, self->position);
		if(min_dist > MIN(min_dist, distance))
		{
			min_dist = distance;
			ent_index = i;
		}
	}
	if(ent_index != -1)
	{

		return &entityList[ent_index];
	}
	else
	{
		return NULL;
	}

}

entity * ent_find_nearest_teammate(entity *self)
{
	int i;
	float min_dist = 99999;
	float distance;
	int ent_index = -1;
	for(i = 0; i < ENTITY_MAX; i++){
		if(!entityList[i].inuse){
			continue;
		}
		if(entityList[i].team != self->team)
		{
			continue;
		}
		if(&entityList[i] == self)
		{
			continue;
		}

		distance = Vec2dDistance(entityList[i].position, self->position);
		if(min_dist > MIN(min_dist, distance))
		{
			min_dist = distance;
			ent_index = i;
		}
	}
	if(ent_index != -1)
	{

		return &entityList[ent_index];
	}
	else
	{
		return NULL;
	}

}

