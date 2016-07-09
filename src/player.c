#include <glib.h>
#include "dict.h"
#include "json_parse.h"
#include "player.h"
#include "particle_emitter.h"
#include "simple_logger.h"
char* player_char_file = "images/player/BODY_male.png";

Entity* player = NULL;

const int PLAYERH = 64; /*<player image height*/
const int PLAYERW = 64; /*<player image width is 64x64.*/
const int PLAYER_FRAMEH = 80;
const int PLAYER_FRAMEW = 80;

static Player_Equip PlayerEquip;
static int anim_current;//used to determine current animation
static int in_build_mode_01 = false;
static int selecting_struct = false;

static GHashTable *g_player_sprites;

extern int g_delta;
//taken from lazyfoo
//order of up,left,down,right are listed in order to match frame vertical in player.png
enum KeyPressSurfaces{
   face_up = 0,
   face_left  = 1,
   face_down = 2,
   face_right = 3,
};
struct Player
{
	int intell;
	int dex;
	int str;
	Sprite *sprite;
};
struct {
	Sprite *image;
	Sprite *image_slash;
	Sprite *image_bow;
	Sprite *image_thrust;
	Sprite *image_spell;
}playerBody;

void Player_Update_Camera();

void Player_Init()
{
	int i;
	Dict	*player_def, 
			*dict_sprites, 
			*dict_bB;

	Sprite	*sprite;

	char *filepath;

	int *imageW, *imageH,
		*frameW, *frameH;
	int *fpl;

	int *health, *mana;

	SDL_Rect boundBox;

	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};

	player_def = Load_Dict_From_File("def/player.def");

	g_player_sprites = g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);

	if(!player_def || player_def->data_type != DICT_HASH)
	{
		slog("Player Def not found");
		return;
	}

	dict_sprites	= Dict_Get_Hash_Value(player_def, "sprites");
	dict_bB			= Dict_Get_Hash_Value(player_def, "boundBox");

	health			= (int *)Dict_Get_Hash_Value(player_def, "health");
	mana			= (int *)Dict_Get_Hash_Value(player_def, "health");

	boundBox.x		= * (int *)(Dict_Get_Hash_Value(dict_bB, "x")->keyValue);
	boundBox.y		= * (int *)(Dict_Get_Hash_Value(dict_bB, "y")->keyValue);
	boundBox.w		= * (int *)(Dict_Get_Hash_Value(dict_bB, "w")->keyValue);
	boundBox.h		= * (int *)(Dict_Get_Hash_Value(dict_bB, "h")->keyValue);

	frameW			= (int *)(Dict_Get_Hash_Value(player_def, "frameW")->keyValue);
	frameH			= (int *)(Dict_Get_Hash_Value(player_def, "frameH")->keyValue);

	//load sprites & animations
	for(i = 0; i < dict_sprites->item_count; i++)
	{
		Line		key;
		Dict		*value;

		value		= dict_get_hash_nth(key, dict_sprites, i);

		filepath	= (char *)(Dict_Get_Hash_Value(value, "filepath")->keyValue);
		
		imageW		= (int *)(Dict_Get_Hash_Value(value, "imageW")->keyValue);
		imageH		= (int *)(Dict_Get_Hash_Value(value, "imageH")->keyValue);

		fpl			= (int *)(Dict_Get_Hash_Value(value, "fpl")->keyValue);

		sprite = Sprite_Load(filepath, *imageW, *imageH, *frameW, *frameH);
		sprite->fpl = *fpl;

		g_hash_table_insert(g_player_sprites, g_strdup(key), sprite);
	}

	sprite = (Sprite *)g_hash_table_lookup(g_player_sprites, "walk");
	player = Entity_load(sprite, pos, *health, *mana, 0);

	player->boundBox = boundBox;

	player->frame = 0;

	player->think = Player_Think;
	player->update = Player_Update;
	
	player->team = TEAM_PLAYER;

	player->p_em = particle_em_new();

	//starting weapon
	PlayerEquip.weapon = getWeapon("longsword");
	player->weapon = PlayerEquip.weapon;
	//armor
	PlayerEquip.head = getArmor("head chain hood");
	PlayerEquip.chest = getArmor("chest chain");

	//should get rid of having to do things this way
	anim_current = WALK;
	
	Player_Update_Camera();
}

void Player_Load_from_Def(Dict *value)
{
	Vec2d pos;
	Vec2d offset;
	int tile_index;
	Tile start;

	char * stored_value;

	Player_Init();

	if(value && value->keyValue)
	{
		stored_value = (char*) value->keyValue;
		tile_index = atoi(stored_value);
		pos = tile_get_pos(tile_index);
			
		offset.x = 10;
		offset.y = 20;

		Vec2dAdd(offset, pos, player->position);
	}
	else
	{
		start = tile_start();

		pos.x = start.mBox.x;
		pos.y = start.mBox.y;
		player->position = pos;
	}

	Player_Update_Camera();

}

void Player_Draw_equip(){
	Vec2d draw_pos;

	Vec2dCopy(player->position, draw_pos);

	if(PlayerEquip.feet){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.feet), player->frame, player->position);
	}
	if(PlayerEquip.hands){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.hands), player->frame, player->position);
	}
	if(PlayerEquip.head){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.head), player->frame, player->position);
	}
	if(PlayerEquip.torso){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.torso), player->frame, player->position);
	}
	if(PlayerEquip.chest){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.chest), player->frame, player->position);
	}
	if(PlayerEquip.shoulders){
		Sprite_Draw(getArmorAnim(anim_current, PlayerEquip.shoulders), player->frame, player->position);
	}

	if(PlayerEquip.weapon && PlayerEquip.weapon->active && anim_current == SLASH ){
		//sword sprites are 192x192 pixels, need offset
		if(PlayerEquip.weapon->type == WEAP_SWORD){
			//offset it
			Vec2dSet(draw_pos, player->position.x - PLAYER_FRAMEW, player->position.y - PLAYER_FRAMEH);

			Sprite_Draw(player->weapon->image, player->frame, draw_pos);
		}
		else{ //draw on player instead
			Sprite_Draw(PlayerEquip.weapon->image, player->frame, player->position);
		} 
	}
}

void Player_Draw()
{
	Entity_Draw(player);

	Player_Draw_equip();
	hud_draw(Camera_Get_Camera(),player->health, player->maxhealth, player->stamina, player->stamina);
}

int player_get_new_frame(int animation, int curr_frame, int fpl)
{
	int frame = curr_frame;

	//check if we're already in a frame of the animation			
	if((frame / fpl) == animation) 
	{	
		//go to next frame in animation
		frame++;				
	}
	//check again to change or reset animation
	if((frame / fpl) != animation)
	{
		//reset to start of anim
		frame = animation * fpl;
	}

	return frame;
}

void Player_Update(Entity *self)
{
	Vec2d new_pos = {player->position.x + player->velocity.x, player->position.y + player->velocity.y};
	int frame;
	int fpl;
	int animation;
	int anim_start_frame;

	frame = player->frame;
	fpl = player->sprite->fpl;	
	animation = player->face_dir;
	anim_start_frame = fpl * animation;

	if(!tile_collision(new_pos, player->boundBox))
	{
		player->position = new_pos;
	}
	player->velocity.x = 0;
	player->velocity.y = 0;

	//don't update walk animation
	//handled in player move
	if(anim_current == WALK)
	{
		return;
	}
	else
	{
		player->next_frame -= g_delta;
		if(player->next_frame <= 0)
		{
			player->frame++;
			player->next_frame = 60;
		}
	}
	
	//if animation finished, reset to walk animation
	if(frame >= (anim_start_frame + fpl - 1))
	{
		anim_current = WALK;
		
		//change to walk sprite
		player->sprite = (Sprite *)g_hash_table_lookup(g_player_sprites, "walk");
		//update frames per line on equips
		fpl = player->sprite->fpl;
		
		player->weapon->active = false;
		
		anim_start_frame = animation * fpl;
		frame = anim_start_frame;

		player->frame = frame;
	}
}

void Player_Update_Camera()
{
	Vec2d pos;

	pos.x = player->position.x - SCREEN_WIDTH / 2;
	pos.y = player->position.y - SCREEN_HEIGHT / 2;

	Camera_SetPosition(pos);
}

void Player_Move(SDL_Event *e){
	
	int frame;
	int fpl;
	int animation; //line on sprite sheet for specific animation
	Vec2d p;

	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}
	
	frame = player->frame;
	fpl = player->sprite->fpl;

	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			player->velocity.y -=5;			
			animation = face_up; 

			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_up;
			break;
        case SDLK_DOWN:
			player->velocity.y +=5;
			animation = face_down;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_down;
			break;
        case SDLK_LEFT:
			player->velocity.x -= 5;
			animation = face_left;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_left;
			break;
        case SDLK_RIGHT:
			player->velocity.x += 5;
			animation = face_right;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
	
			player->face_dir = face_right;
			break;
		case SDLK_q:
			slog("Q press");
			p.y = player->position.y + player->sprite->frameH * 3 / 4;
			p.x = player->position.x;
					
			particle_em_add(player->p_em, PARTICLE_SPELLCAST, p);
			break;
		case SDLK_e:
			set_hud_state(HUD_state::main_menu);
			in_build_mode_01 = Bool_False;
			break;
		case SDLK_i:
			set_hud_state(HUD_state::inventory1);
			break;
		case SDLK_b:
			set_hud_state(HUD_state::build1);
			in_build_mode_01 = Bool_True;
			break;
		case SDLK_v:
			if(in_build_mode_01)
			{
				structure_spawn( Entity_TYPE::WALL );

				in_build_mode_01 = Bool_False;
				set_hud_state(HUD_state::main_menu);
			}
			break;
		case SDLK_f:
			slog("Press F");
			if(anim_current != SLASH && Player_Tree_Collision())
			{
				anim_current = SLASH;
				animation = player->face_dir;
				///player->sprite->fpl = playerBody.image_slash->fpl;
				player->sprite = (Sprite *) g_hash_table_lookup(g_player_sprites, "slash");
				fpl = player->sprite->fpl;

				player->frame = animation * fpl; // go back to start of animation					
				player->next_frame = 50;
			}
			break;
		case SDLK_SPACE:			
			//if(player_struct.weapon == WEAP_SWORD)
			if(anim_current != SLASH)
			{
				Sprite *new_sprite = (Sprite *)g_hash_table_lookup(g_player_sprites, "slash");			
				player->sprite = new_sprite;				

				//weapon sprites have fewer frames than player slash anim
				//set new player sprite to have fewer fpl
				fpl = PlayerEquip.weapon->fpl;
				new_sprite->fpl = fpl;
				
				animation = player->face_dir;
				player->frame = fpl * animation; // start frame on animation matching direction
				player->next_frame = 50;
				anim_current = SLASH;
				player->weapon->active = true;				
																				
				//check if we hit enemy
				weapon_collision(player);
			}
			break;
		default:
			break;
	}
	Player_Update_Camera();
}

int Player_Tree_Collision()
{
	if(!player)
	{ 
		slog("No player or face direction");
		return false;
	}

	return tile_forage(player->position, player->boundBox, player->face_dir);
}
void Player_Think(Entity* self)
{


}