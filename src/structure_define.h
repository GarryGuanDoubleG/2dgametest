#ifndef _STRUCT_DEFINE_H_
#define _STRUCT_DEFINE_H_

enum struct_type{
	main_base01,
	wall_01,
	tower_01,
};

#define SPRITE_MAIN_BASE01_FILEPATH "images/structures/main_base01.png"
#define SPRITE_IMAGE_W 160
#define SPRITE_IMAGE_H 183
#define SPRITE_FRAME_W TILE_WIDTH  * 2
#define SPRITE_FRAME_H TILE_HEIGHT * 2

#define SPRITE_WALL01_FILEPATH "images/structures/wall_01.png"
#define SPRITE_IMAGE_W 66
#define SPRITE_IMAGE_H 160
#define SPRITE_FRAME_W TILE_WIDTH  * 2
#define SPRITE_FRAME_H TILE_HEIGHT * 2

#define SPRITE_TOWER01_FILEPATH "images/structures/tower_01.png"
#define SPRITE_IMAGE_W 100
#define SPRITE_IMAGE_H 160
#define SPRITE_FRAME_W TILE_WIDTH  * 2
#define SPRITE_FRAME_H TILE_HEIGHT * 2


#endif