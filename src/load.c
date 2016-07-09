#include "load.h"
#include "json_parse.h"


Dict * Load_Dict_From_File(char * filename)
{
	if(!filename) return NULL;
	return json_parse_file(filename);
}