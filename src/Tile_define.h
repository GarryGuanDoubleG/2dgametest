#ifndef _TILE_DEFINE_H_
#define _TILE_DEFINE_H_

//destructable tiles
#define TILE_TREE_W 97
#define TILE_TREE_H 126

typedef enum tile_type_S
{
	TILE_GRASS = 0,
	TILE_TREE = 1,
	TILE_ROAD = 2,
	TILE_WATER = 3,
	TILE_BUSH = 4
}tile_types;

#define PATH_TILE_TREE "images/terrain/terrain_atlas_tree.png"
#define PATH_TILE_GRASS "images/terrain/Grass01.png"
#define PATH_TILE_BUSH "images/terrain/bush.png"
#define PATH_TILE_WATER "images/terrain/water_pit.png"
#endif