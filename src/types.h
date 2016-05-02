#include <SDL.h>
typedef char Line[128];
typedef Uint8 Bool; 

enum BoolEnum
{
	Bool_True = 1,
	Bool_False = 0,
};

int Bool_from_string( char * string);
char *string_from_bool(Bool b);
