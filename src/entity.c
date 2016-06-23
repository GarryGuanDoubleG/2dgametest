#include "Entity.h"

#include "simple_logger.h"

const int		g_entity_max = 2000; // max entities allocated into memory
int				g_entity_count = 0;

Entity			*EntityList; // handle on all entities
Dict			*Entity_defs = NULL;// dictionary of all Entity definitions

extern			Entity *player;

void Entity_initialize_system(){
	int i;
	Line key;
	Dict * temp;

	EntityList = (Entity *)malloc(sizeof(Entity) * g_entity_max);
	memset(EntityList, 0, sizeof(Entity)* g_entity_max);

	for(i = 0; i < g_entity_max; i++)
	{
		EntityList[i].sprite = NULL;
		EntityList[i].inuse = false;
	}

	Entity_defs = load_dict_from_file("def/test.def");

	if(!Entity_defs)
	{
		slog("Error, could not load Entity def");
	}
	else if(Entity_defs->data_type != DICT_HASH)
	{
		slog("Def file is not a hash");
	}
	else
	{
		for(i = 0; i < Entity_defs->item_count; i++)
		{
			temp = dict_get_hash_nth(key, Entity_defs, i);
			slog("%i: \n%s \n%s", i, key, (const char*)temp->keyValue);
		}
	}

	atexit(Entity_close);
}

Entity * Entity_New(){
	int i;

	if(++g_entity_count >= g_entity_max)
	{
		slog("Max entities have already been reached");
		return NULL;
	}

	for(i = 0; i < g_entity_max; i++)
	{

		if(EntityList[i].inuse == true)
		{
			continue;
		}

		memset(&EntityList[i], 0, sizeof(Entity));
		EntityList[i].inuse = true;

		return (&EntityList[i]);
	}

	return NULL;
}
//loads structure
Entity* struct_load(Sprite *sprite, int health, int defense, int type)
{
	int i;
	for(i = 0; i < g_entity_count + 1; i++)
	{
		 if(EntityList[i].inuse == false)
		 {
			 g_entity_count++;
			 EntityList[i].inuse = true;
			 EntityList[i].sprite = sprite;
			 EntityList[i].health = health;
			 EntityList[i].maxhealth = health;			 
			 EntityList[i].structure = true;
			 EntityList[i].placed = false;
			 EntityList[i].selected = false;
			 
			 return &EntityList[i];
			 break;
		 }
	}
	return NULL;
}

Entity* Entity_load(Sprite *sprite,Vec2d pos, int health, int stamina, int state){
	int i;
	for(i = 0; i < g_entity_count + 1; i++)
	{
		 if(EntityList[i].inuse == false)
		 {
			 g_entity_count++;			
			 EntityList[i].inuse = true;
			 EntityList[i].sprite = sprite;
			 EntityList[i].position = pos;
			 EntityList[i].health = health;
			 EntityList[i].maxhealth = health;
			 EntityList[i].stamina = stamina;
			 EntityList[i].state = state;
			 EntityList[i].followPath = ent_follow_path;
			 EntityList[i].id = i;
			 EntityList[i].path = NULL;
			 return &EntityList[i];
			 break;
		 }
	}
	return NULL;
}

//code based on post by MikeyPro on sdl forum
//link: https://forums.libsdl.org/viewtopic.php?t=11040&sid=0c796abe9954dba84ee2814c32c85f5c

void draw_health_bar(Entity *self){
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
	if(!rect_collide(bg_rect, Camera_Get_Camera()))
	{
		SDL_SetRenderDrawColor(Graphics_Get_Renderer(), 0x00, 0x00, 0x00, 0x00);
		return;
	}
		bg_rect.x -= Camera_Get_Camera().x;
		bg_rect.y -= Camera_Get_Camera().y;
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

void Entity_Draw(Entity *ent){
	//SDL Main Camera src rect	
	Sprite_Draw(ent->sprite, ent->frame, ent->position);
	if(ent != player)
	{
		draw_health_bar(ent);
	}
}

void Entity_Free(Entity* ent){ //makes Entity->inuse false so new ent can be initialized in this mem location
	ent->inuse = false;
	ent->sprite = NULL;
	path_free(ent->path);
	g_entity_count--;

}

void Entity_close(){ //deallocates all entities
	int i;
	for(i = 0; i < g_entity_max; i++){
		 if(EntityList[i].inuse == true){
			 Entity_Free(&EntityList[i]);	
		 }
	}
	memset(EntityList, 0, sizeof(Entity) * g_entity_max);
}

void Entity_think_all(){
	int i = 0;
	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(!EntityList[i].think){
			continue;
		}
		/*if(EntityList[i].path)
		{
			path_free(EntityList[i].path);
		}*/
		EntityList[i].think(&EntityList[i]);
	}
}

void Entity_update_all(){
	int i = 0;
	Vec2d new_pos;
	Vec2d old_pos;
	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(!EntityList[i].sprite)
		{
			Entity_Free(&EntityList[i]);
			continue;
		}
		//check for map bounds
		if(EntityList[i].position.x < 0 ) 
		{
			EntityList[i].position.x =0;
		}
		if(EntityList[i].position.x + EntityList[i].sprite->frameW > TILE_ROWS * TILE_WIDTH)
		{
			EntityList[i].position.x  = TILE_ROWS * TILE_WIDTH - EntityList[i].sprite->frameW;
		}
		if(EntityList[i].position.y < 0 ) 
		{
			EntityList[i].position.y =0;
		}
		if(EntityList[i].position.y + EntityList[i].sprite->frameH > TILE_COLUMNS * TILE_HEIGHT)
		{
			EntityList[i].position.y  = TILE_COLUMNS * TILE_HEIGHT - EntityList[i].sprite->frameH;
		}

		if(!EntityList[i].update){
			continue;
		}

		EntityList[i].update(&EntityList[i]);
		//don't free player
		if(EntityList[i].health <= 0 && !(&EntityList[i] == player))
		{
			Entity_Free(&EntityList[i]);
		}
		if(EntityList[i].weapon){
			EntityList[i].weapon->face_dir = EntityList[i].face_dir;
		}
		if(&EntityList[i] != player)
		{		
			Entity_Draw(&EntityList[i]);
		}
	}
}

void Entity_death(Entity *self){
	//death animation

	//free
	Entity_Free(self);
}

void weapon_touch(Entity * self, Entity *other)
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
		Entity_death(other);
	}

}
int Entity_check_collision(Entity * self)
{
	int i;
	for(i = 0; i < g_entity_count + 1; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(&EntityList[i] == self)
		{
			continue;
		}
		if(Entity_collide(self, &EntityList[i]))
		{
			return true;
		}
	}
	return false;
}
int Entity_collide(Entity *a, Entity*b)
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
int weapon_collision(Entity *self, Entity *other, Rect_f bound)
{
	Rect_f aB = {(float)other->position.x + (float)other->boundBox.x, (float)other->position.y + other->boundBox.y, other->boundBox.w, other->boundBox.h};
	Rect_f bB = {(float)self->position.x + bound.x + self->weapon->boundOffset.x, (float)self->position.y + bound.y + self->weapon->boundOffset.y, bound.w, bound.h};

	if(!self || !self->weapon || !other){
		slog("NO weap or Entity");
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

void weapon_collision(Entity *owner)
{
	int i;

	if(!owner || !owner->weapon)
	{
		slog("Entity or Weapon is NULL. No Collision");
	}
	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(!EntityList[i].update){
			continue;
		}
		if(owner == &EntityList[i]){
			continue;
		}
		if(owner->weapon && owner->weapon->active){
			switch(owner->weapon->face_dir){
				case UP:
						weapon_collision(owner, &EntityList[i], owner->weapon->boundUp);
						break;
				case DOWN:
						weapon_collision(owner, &EntityList[i], owner->weapon->boundDown);
						break;
				case LEFT:
						weapon_collision(owner, &EntityList[i], owner->weapon->boundLeft);
						break;
				case RIGHT:
						weapon_collision(owner, &EntityList[i], owner->weapon->boundRight);
						break;
				default:
					break;
			}
		}
	}
}

void Entity_check_collision_all()
{
	int i = 0;
	int j = 0;
	Entity *curr = NULL;
	Entity *next = NULL;

	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(!EntityList[i].update){
			continue;
		}
		curr = &EntityList[i];

		for(j = 0; j < g_entity_max; j++){
			if(!EntityList[j].inuse){
				continue;
			}
			if(!EntityList[j].update){
				continue;
			}
			if(curr == &EntityList[j]){
				continue;
			}
			if(!curr){
				slog("Curr is null");
				continue;
			}
			next = &EntityList[j];
			if((!next || !curr) || curr == next )
			{
				continue;
			}
			if(Entity_collide(curr, next))
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
Entity * Entity_Get_Player()
{
	return player;
}

void Entity_Move_Towards_Player(Entity *ent, int speed)
{
	Entity *player;

	Vec2d direction;
	Vec2d player_pos;
	Vec2d new_vel;
	Vec2d unit_vec;

	float distance;

	player = Entity_Get_Player();

	direction.x = player->position.x - ent->position.x;
	direction.y = player->position.y - ent->position.y;

	distance = Normalize2d(direction);
	VectorScale(&direction, &new_vel, speed);

	ent->velocity = distance != 0 ? new_vel : ent ->velocity;
	path_free(ent->path);
	ent->path = NULL;
}

void Entity_Set_Random_Velocity(Entity *ent, int speed)
{
	Vec2d new_vel;

	int randomNum = rand() % 15;

	if(!ent)
	{
		slog("Ent is null");
		return;
	}

	new_vel.x = randomNum <= 5 ? speed :
				randomNum <= 10 ? -speed : 0;

	randomNum = rand() % 15;

	new_vel.y = randomNum <= 5 ? speed :
				randomNum <= 10 ? -speed : 0;

	ent->velocity = new_vel;
}


void ent_follow_path(Entity *self)
{
	Vec2d tile_pos;
	Vec2d tile_center_pos;
	Vec2d self_center_pos = { ENT_CENTER_X(self), ENT_CENTER_Y(self)};
	Vec2d player_center_pos = { ENT_CENTER_X(Entity_Get_Player()), ENT_CENTER_Y(Entity_Get_Player()) };
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
		return;
	}
	else
	{
		 tile_pos = tile_get_pos(self->path->tile_index);

		 tile_center_pos.x = TILE_CENTER_X(tile_pos);
		 tile_center_pos.y = TILE_CENTER_Y(tile_pos);

		 tile_bound.x = tile_center_pos.x;
		 tile_bound.y = tile_center_pos.y;
	}

	if(rect_collide(tile_bound, self_bound))
	{
		path_free_node(&(self->path));
		self->velocity.x = 0;
		self->velocity.y = 0;
	}
	else
	{	
		Vec2dSubtract(tile_center_pos, self_center_pos, new_vel);
		Normalize2d(new_vel);
		VectorScale(&new_vel, &new_vel, 5);
		x = new_vel.x;
		y = new_vel.y;
		while((x * x + y * y) < 49)
		{
			if(x == 0 && y == 0)
				break;
			x *= 2;
			y *= 2;
		}
		new_vel.x = x;
		new_vel.y = y;
		self->velocity = new_vel;
	//	slog("New Vel X:%f Y:%f", new_vel.x, new_vel.y);
	}

}

Entity * ent_find_nearest_enemy(Entity *self)
{
	int i;
	float min_dist = 99999;
	float distance;
	int ent_index = -1;
	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(EntityList[i].team == self->team)
		{
			continue;
		}
		if(&EntityList[i] == self)
		{
			continue;
		}

		distance = Vec2dDistance(EntityList[i].position, self->position);
		if(min_dist > MIN(min_dist, distance))
		{
			min_dist = distance;
			ent_index = i;
		}
	}
	if(ent_index != -1)
	{

		return &EntityList[ent_index];
	}
	else
	{
		return NULL;
	}

}

Entity * ent_find_nearest_teammate(Entity *self)
{
	int i;
	float min_dist = 99999;
	float distance;
	int ent_index = -1;
	for(i = 0; i < g_entity_max; i++){
		if(!EntityList[i].inuse){
			continue;
		}
		if(EntityList[i].team != self->team)
		{
			continue;
		}
		if(&EntityList[i] == self)
		{
			continue;
		}

		distance = Vec2dDistance(EntityList[i].position, self->position);
		if(min_dist > MIN(min_dist, distance))
		{
			min_dist = distance;
			ent_index = i;
		}
	}
	if(ent_index != -1)
	{

		return &EntityList[ent_index];
	}
	else
	{
		return NULL;
	}

}

