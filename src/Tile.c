#include "Tile.h"
#include "simple_logger.h"
#include <time.h>
#include <math.h>

const int TILE_WIDTH = PLAYER_FRAMEH;
const int TILE_HEIGHT = PLAYER_FRAMEW;
const int TOTAL_TILES_X = SCREEN_WIDTH/TILE_WIDTH * 3;
const int TOTAL_TILES_Y = SCREEN_HEIGHT/TILE_HEIGHT * 3;
const int TOTAL_TILES = TOTAL_TILES_X * TOTAL_TILES_Y ;//make a square 3 by 3 region of size screen

Tile *tile_list = NULL;
Destructable_Tile *dest_tile_list = NULL;
Sprite2 * tile_sprite_grass = NULL;
Sprite2 * tile_sprite_tree = NULL;


void tile_init_system(){
	int i;
	if(TOTAL_TILES == 0){
		printf("Why is total tiles 0?");
		return;
	}
	tile_list = (Tile*) malloc(sizeof(Tile)*TOTAL_TILES);
	memset(tile_list,0, sizeof(Tile) * TOTAL_TILES);

	dest_tile_list = (Destructable_Tile*) malloc(sizeof(Destructable_Tile)*TOTAL_TILES);
	memset(dest_tile_list,0, sizeof(Destructable_Tile) * TOTAL_TILES);

	tile_sprite_grass = tile_load(PATH_TILE_GRASS);
	tile_sprite_tree = tile_load(PATH_TILE_TREE);
	tile_set();
	tile_render();
	atexit(tile_close_system);
}

Sprite2 * tile_load(char *filename)
{
	Sprite2 * sprite = sprite_load(filename, TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	return sprite;
}

int score_move(int move)
{
	int score = 0;

	if( TILE_CAN_MOVE_UP(move))
	{
		if(tile_list[move - TOTAL_TILES_X].mType == TILE_ROAD)
		{
			score += 2;
		}
	}
	else{ score++; }  // edge tile increases score by 1

	if(TILE_CAN_MOVE_DOWN(move))
	{
		if(tile_list[move + TOTAL_TILES_X].mType == TILE_ROAD)
		{
			score +=2;
		}
	}
	else { score++; }

	if(TILE_CAN_MOVE_LEFT(move))
	{
		if(tile_list[move - 1].mType == TILE_ROAD)
		{
			score +=2;
		}
	}
	else {score++;}
	if(TILE_CAN_MOVE_RIGHT(move))
	{
		if(tile_list[move + 1].mType == TILE_ROAD)
		{
			score +=2;
		}
	}
	else {score++;}
	
	if(tile_list[move].mType == TILE_ROAD)
	{
		score += 4;
	}

	return score;
}
int tile_forest_walk(int moves[])
{
	int min;
	int score[] = {-1,-1,-1,-1};
	int new_node = -1;
	int i;
	
	score[0] = score_move(moves[0]);
	score[1] = score_move(moves[1]);
	score[2] = score_move(moves[2]);
	score[3] = score_move(moves[3]);
	
	min = score[0];
	min = MIN(min, score[1]);
	min = MIN(min, score[2]);
	min = MIN(min, score[3]);

	for(i = 0; i < 4; i++)//4 is number of moves
	{
		if(score[i] == min)
		{
			if(new_node == -1)
			{
				new_node = moves[i];
			}
			else
			{
				int a = 0, b = 0;
				int random_num;
				a = DISTANCE_CENTER(new_node);
				b = DISTANCE_CENTER(moves[i]);
				//higher chance if closer to center
				if(a < b)
				{
					//give smaller num 66% chance of being new move
					random_num = rand() % 9;
					new_node = random_num <= 5 ? new_node : moves[i];
				}
				else if ( b < a)
				{
					random_num = rand() % 9;
					new_node = random_num <=5 ? moves[i] : new_node;
				}
				else
				{
					random_num = rand() % 1;
					new_node = random_num == 0 ? moves[i] : new_node;
				}
			}
		}
	}

	return new_node;

}
//applies to whole map
void tile_forest_gen(int start)
{
	int i = start;
	int done = false;
	int lifespan = TOTAL_TILES/10;
	//-1 means can't move to that node
	tile_list[i].mType = TILE_ROAD;
	dest_tile_list[i].mType = TILE_ROAD;

	while(lifespan-- > 0)
	{
		int moves[4] = {i-1, i+1, i-TOTAL_TILES_X, i + TOTAL_TILES_X}; // left,right,up,down a tile on map
		if(!TILE_CAN_MOVE_LEFT(i))
		{
			moves[0] = -1;
		}
		if(!TILE_CAN_MOVE_RIGHT(i))
		{
			moves[1] = -1;
		}
		if(!TILE_CAN_MOVE_UP(i))
		{
			moves[2] = -1;
		}
		if(!TILE_CAN_MOVE_DOWN(i))
		{
			moves[3] = -1;
		}
		i = tile_forest_walk(moves);
		tile_list[i].mType = TILE_ROAD;
		dest_tile_list[i].mType = TILE_ROAD;
	}
}

void tile_forest_gen()
{
	int start;
	
	start = 0;
	tile_forest_gen(start);
	//start of top edge of forest
	start = rand() % TOTAL_TILES_X -1; //rand num from 0 to end of first row
	tile_forest_gen(start);
	//start on left edge of forest
	start = rand() % TOTAL_TILES_Y + 1;
	start = start >= TOTAL_TILES_Y ? TOTAL_TILES_Y - 1 : start;
	start = start * TOTAL_TILES_X;
	tile_forest_gen(start);
	//right edge
	start = rand() % TOTAL_TILES_Y +1;
	start = start >= TOTAL_TILES_Y ? TOTAL_TILES_Y - 1 : start;
	start = (start *TOTAL_TILES_X) - 1;
	tile_forest_gen(start);
	//bottom edge
	start = rand() % TOTAL_TILES_X -1;
	start += TOTAL_TILES - TOTAL_TILES_X;
	tile_forest_gen(start);
}

void slog_dest_tree_list()
{
	int i;
	for(i = 0; i < 120; i++)
	{
		slog( "Index:%i TYPE:%i", i, dest_tile_list[i].mType);
	}
}

void tile_set(){
	int i;
	int x = 0,y = 0;
	Tile * tile;
	//place the grass tile
	for( i = 0; i < TOTAL_TILES; i++){

		tile = (tile_list+i);
		tile->mBox.x = x;
		tile->mBox.y = y;
		tile->mBox.w = TILE_WIDTH;
		tile->mBox.h = TILE_HEIGHT;
		tile->mType = TILE_GRASS1;//filler
		
		dest_tile_list[i].mBox = tile->mBox;
		dest_tile_list[i].mType = TILE_TREE;
		dest_tile_list[i].hits = 5;
		x += TILE_WIDTH;		
		if(x >= TOTAL_TILES_X * TILE_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		} 
	}
	tile_forest_gen();

	slog_dest_tree_list();
}

void tile_free(Tile *tile){
	tile->mType = 0;
}

int tile_get_type(int index)
{
	if(index < 0 || index > TOTAL_TILES)
	{
		return -1;
	}
	return dest_tile_list[index].mType;
}
int tile_to_tile_dist(int tile_1, int tile_2)
{
	int move_left = 0, move_right = 0, move_down = 0, move_up = 0;
	int start = tile_1;
	Vec2d t1_pos = tile_get_pos(tile_1);
	Vec2d t2_pos = tile_get_pos(tile_2);

	if(t2_pos.x < t1_pos.x)
	{
		while(tile_1 % (TOTAL_TILES_X) != 0 && tile_get_pos(tile_2).x <  tile_get_pos(tile_1).x )
		{
			tile_1--;
			move_left++;
		}
		tile_1 = start;
	}
	else if(t2_pos.x > t1_pos.x)
	{
		while(tile_1 %(TOTAL_TILES_X-1) != 0 && tile_get_pos(tile_2).x >  tile_get_pos(tile_1).x )
		{
			tile_1--;
			move_right++;
		}
		tile_1 = start;
	}
	if(t2_pos.y < t1_pos.y)
	{
		while(tile_1 > (TOTAL_TILES_X) && tile_get_pos(tile_2).y <  tile_get_pos(tile_1).y)
		{
			tile_1 -= TOTAL_TILES_X;
			move_up++;
		}
		tile_1 = start;
	}
	if(t2_pos.y > t1_pos.y)
	while(tile_1 < (TOTAL_TILES - TOTAL_TILES_X) && tile_get_pos(tile_2).y >  tile_get_pos(tile_1).y)
	{
		tile_1 += TOTAL_TILES_X;
		move_down++;
	}
	
	return move_left + move_right + move_up + move_down;
}

void tile_render(){
	int i;
	SDL_Rect camera = graphics_get_player_cam();
	for( i = 0; i < TOTAL_TILES; i++){
		Tile * tile = (tile_list + i);
		if(tile == NULL){
			printf("Tile is null while rendering");
			return ;
		}
		if(rect_collide(graphics_get_player_cam(), tile->mBox))
		{
			SDL_Rect dest = { tile->mBox.x - camera.x,tile->mBox.y-camera.y, TILE_HEIGHT, TILE_WIDTH};
			SDL_RenderCopy(__gt_graphics_renderer,tile_sprite_grass->image, NULL, &dest);
			if(dest_tile_list[i].mType == TILE_TREE)
			{
				SDL_RenderCopy(__gt_graphics_renderer,tile_sprite_tree->image, NULL, &dest);
			}
		}
	}
}

void tile_close_system(){
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
	{
		tile_free(tile_list+i);
	}
	memset(tile_list, 0, sizeof(Tile) * TOTAL_TILES);
	memset(tile_list, 0, sizeof(Destructable_Tile) * TOTAL_TILES);

	SDL_DestroyTexture(tile_sprite_grass->image);
	SDL_DestroyTexture(tile_sprite_tree->image);
}

Tile tile_start()
{

	int i;
	for( i = 0; i < TOTAL_TILES; i++)
	{
		if(tile_list[i].mType == TILE_ROAD)
		{
			
			if(rand() % 20 <= 1 && i != 0)
			{
				return tile_list[i];
			}
		}
	}

	return tile_list[0];
}

void tile_make_bound(Rect_f structure)
{
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
	{
		if(dest_tile_list[i].mType != TILE_TREE && 
			rect_collide(structure, dest_tile_list[i].mBox))
		{
			dest_tile_list[i].mType = TILE_TREE;//here for collision. need to make another type
		}
	}
}

int tile_structure_collision(Rect_f structure)
{
	
	int i;
	for( i = 0; i < TOTAL_TILES; i++)
	{
		if(dest_tile_list[i].mType == TILE_TREE && 
			rect_collide(structure, dest_tile_list[i].mBox))
		{
			return true;
		}
	}
	return false;
}


int tile_collision(Vec2d pos, SDL_Rect bound)
{
	int i;
	Rect_f player_pos = {pos.x + bound.x, pos.y + bound.y, bound.w, bound.h};
	Rect_f tile_bound;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		tile_bound.x = tile_list[i].mBox.x;
		tile_bound.y = tile_list[i].mBox.y;
		tile_bound.w = tile_list[i].mBox.w;
		tile_bound.h = tile_list[i].mBox.h;

		if(rect_collide(player_pos, tile_bound))
		{
			if(dest_tile_list[i].mType == TILE_TREE)
			{
				return true;
			}
		}
	}
	return false;
}

int tile_forage(Vec2d pos, SDL_Rect bound, int face_dir)
{
	int i;
	int curr_tile;
	int is_tree = false;
	int tree_index;
	Rect_f player_pos = {pos.x + bound.x+bound.w/2, pos.y + bound.y + bound.h/2, bound.w/2, bound.h/2};
	Rect_f tile_bound;

	for( i = 0; i < TOTAL_TILES; i++)
	{
		tile_bound.x = tile_list[i].mBox.x;
		tile_bound.y = tile_list[i].mBox.y;
		tile_bound.w = tile_list[i].mBox.w;
		tile_bound.h = tile_list[i].mBox.h;
		
		if(rect_collide(player_pos, tile_bound))
		{
			curr_tile = i;
			break;
		}
	}

	switch(face_dir)
	{
		case UP:
			slog("Up");
			if(curr_tile < TOTAL_TILES_X)
			{
				return false;
			}
			if (dest_tile_list[curr_tile - TOTAL_TILES_X].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile - TOTAL_TILES_X;
			}
			break;
		case DOWN:
			slog("Down");
			if(curr_tile > TOTAL_TILES - TOTAL_TILES_X)
			{
				return false;
			}
			if(dest_tile_list[curr_tile + TOTAL_TILES_X].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile + TOTAL_TILES_X;
			}
			break;
		case LEFT:
			slog("Left");
			if(curr_tile % TOTAL_TILES_X == 0)
			{
				return false;
			}
			if (dest_tile_list[curr_tile - 1].mType == TILE_TREE)
			{
				is_tree = true;
				tree_index = curr_tile - 1;
			}
			break;
		case RIGHT:
			slog("Right");
			if(curr_tile % (TOTAL_TILES_X -1 ) == 0)
			{
				return false;
			}
			if(dest_tile_list[curr_tile + 1].mType == TILE_TREE)
			{
				tree_index = curr_tile + 1;
				is_tree = true;
			}
			break;
		default:
			break;
	}

	if(is_tree)
	{
		dest_tile_list[tree_index].hits -= 1;
		if( dest_tile_list[tree_index].hits == 0)
		{
			dest_tile_list[tree_index].mType = NULL;
			inventory_add("wood");
		}
	}
	slog("Is tree is %s", is_tree ? "TRUE" : "False");
	return is_tree;
}


int tile_get_tile_number(Vec2d pos, SDL_Rect bound)
{
	int i;
	Vec2d bound_pos = {pos.x + bound.x + bound.w/2, pos.y + bound.y + bound.h/2};
	Rect_f tile_box;

	for(i = 0; i < TOTAL_TILES; i++)
	{
		tile_box.x = tile_list[i].mBox.x;
		tile_box.y = tile_list[i].mBox.y;
		tile_box.w = tile_list[i].mBox.w;
		tile_box.h = tile_list[i].mBox.h;

		if(bound_pos.x >= tile_box.x && bound_pos.x <= tile_box.x + tile_box.w &&
			bound_pos.y >= tile_box.y && bound_pos.y <= tile_box.y + tile_box.h )
		{
			return i;
		}
	}

	return -1;
}

void slog_heuristic(int size, int start, int target, tile_heuristic *tile_list)
{
	int i;

	if(!tile_list)
	{
		slog("Tile Heuristic is NULL");
	}

	for(i = 0; i < (size); i++)
	{
		slog("Tile Heuristic Index: %i Score is %i", tile_list->tile_index[i], tile_list->distance_to_target[i]);
		slog("Start: %i Target:%i", start, target);
	}
}

void tile_list_heuristic_free(tile_heuristic * tile_list)
{
	if(!tile_list)
	{
		return;
	}
	if(tile_list->distance_to_target)
	{
		delete(tile_list->distance_to_target);
	}
	if(tile_list->tile_index)
	{
		delete(tile_list->tile_index);
	}
	if(tile_list)
	{
		delete(tile_list);
	}
}

float tile_dist_to_target(int start, int target)
{
	Vec2d vec_start = { tile_list[start].mBox.x, tile_list[start].mBox.y };
	Vec2d vec_target = { tile_list[target].mBox.x, tile_list[target].mBox.y };
	return Vec2dDistance(vec_start, vec_target);
}

Vec2d tile_get_pos(int index)
{
	Vec2d tile_pos = {tile_list[index].mBox.x, tile_list[index].mBox.y };

	return tile_pos;	
}