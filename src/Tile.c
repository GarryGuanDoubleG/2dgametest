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

	if( move > TOTAL_TILES_X -1)
	{
		if(tile_list[move - TOTAL_TILES_X].mType == TILE_ROAD)
		{
			score += 2;
		}
	}
	else{ score++; }  // edge tile increases score by 1

	if( move < TOTAL_TILES - TOTAL_TILES_X -1)
	{
		if(tile_list[move + TOTAL_TILES_X].mType == TILE_ROAD)
		{
			score +=2;
		}
	}
	else { score++; }

	if(move % TOTAL_TILES_X != 0)
	{
		if(tile_list[move - 1].mType == TILE_ROAD)
		{
			score +=2;
		}
	}
	else {score++;}
	if(move % (TOTAL_TILES_X - 1) != 0)
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
	int lifespan = TOTAL_TILES/2;
	//-1 means can't move to that node
	tile_list[i].mType = TILE_ROAD;
	dest_tile_list[i].mType = TILE_ROAD;

	while(lifespan-- > 0)
	{
		int moves[4] = {i-1,i+1,i-TOTAL_TILES_X,i + TOTAL_TILES_X}; // left,right,up,down a tile on map
		if(i%TOTAL_TILES_X == 0)
		{
			moves[0] = -1;
		}
		if((i + 1)%TOTAL_TILES_X == 0)
		{
			moves[1] = -1;
		}
		if(i < TOTAL_TILES_X == 0)
		{
			moves[2] = -1;
		}
		if( i > TOTAL_TILES - TOTAL_TILES_X)
		{
			moves[3] = -1;
		}
		i = tile_forest_walk(moves);
		tile_list[i].mType = i;
		dest_tile_list[i].mType = i;
	}
}

void tile_forest_gen()
{
	int start;
	srand(time(NULL));
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
			slog("Total Tiles X is < X");
			y += TILE_HEIGHT;
		} 
	}
	tile_forest_gen();
}

void tile_free(Tile *tile){
	tile->mType = 0;
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
			return tile_list[i];
		}
	}
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
				return false;
			}
		}
	}
	return true;
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

int tile_get_tile_number(Vec2d pos)
{
	int i;
	Rect_f self_pos = { pos.x, pos.y, 0,0};
	Rect_f tile_box;
	for(i = 0; i < TOTAL_TILES; i++)
	{
		tile_box.x = tile_list[i].mBox.x;
		tile_box.y = tile_list[i].mBox.y;
		tile_box.w = tile_list[i].mBox.w;
		tile_box.h = tile_list[i].mBox.h;

		if(rect_collide(tile_box, self_pos))
		{
			return i;
		}
	}

}

void slog_heuristic(int size, int **tile_list)
{
	int i;

	if(!tile_list)
	{
		slog("Tile Heuristic is NULL");
	}

	for(i = 0; i < (size * size); i++)
	{
		slog("Tile Heuristic Index: %i Score is %i", tile_list[i][0], tile_list[i][1]);
	}
}
int ** tile_get_heuristic(int start, int target)
{
	const int side = 20; //20 by 20
	int move_left = 0;
	int move_right = 0;
	int move_up = 0;
	int move_down = 0; 
	int tile_num = start;
	int i;
	int j;
	int tile_index;
	int **tile_list; // tile index and heuristic score
	
	while(tile_num %(TOTAL_TILES_X) != 0 && move_left < side/2)
	{
		tile_num--;
		move_left++;
	}
	tile_num = start;

	while(tile_num %(TOTAL_TILES_X-1) != 0 && move_right < side/2)
	{
		tile_num--;
		move_right++;
	}
	tile_num = start;

	while(tile_num > (TOTAL_TILES_X) && move_up < side/2)
	{
		tile_num -= TOTAL_TILES_X;
		move_up++;
	}
	tile_num = start;
	while(tile_num < (TOTAL_TILES - TOTAL_TILES_X) && move_down < side/2)
	{
		tile_num += TOTAL_TILES_X;
		move_down++;
	}

	tile_index = start - move_left - (TOTAL_TILES_X * move_up);

	for(j = 0; j < (move_up + move_down); j++)//traverse vertical tile list
	{
		for(i = 0; i < move_left + move_right; i++) // traverse horizontal tile list
		{
			int count = 0; // heuristic move count
			int temp_index = tile_index;
			int found = false;
			if(tile_index % TOTAL_TILES_X > (target % TOTAL_TILES_X))// each tile index moves to target and counts
			{
				while(temp_index % TOTAL_TILES_X < target % TOTAL_TILES_X)
				{
					temp_index++;
					count++;
				}
			}
			else
			{
				while(temp_index % TOTAL_TILES_X > target % TOTAL_TILES_X)
				{
					temp_index--;
					count++;
				}
			}
			while(temp_index != target) // check if target is in same row as start
			{
				if(temp_index > target)
				{
					temp_index -= TOTAL_TILES_X;
					count++;
				}
				else
				{
					temp_index += TOTAL_TILES_X;
					count++;
				}
			}
			tile_list[i+j][0] = tile_index;
			tile_list[i+j][1] = count;

		}
		tile_index = start - move_left - (TOTAL_TILES_X * (move_up - j));
	}

	slog_heuristic(side, tile_list);
	return tile_list;
}