#include "load.h"
#include "json_parse.h"


Dict * load_dict_from_file(char * filename)
{
	if(!filename) return NULL;
	return json_parse_file(filename);
}