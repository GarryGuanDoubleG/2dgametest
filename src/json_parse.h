#ifndef _JSON_PARSE_H_
#define _JSON_PARSE_H_
#include <SDL.h>
#include <glib.h>
#include "dict.h"

Dict * json_parse_file(char *filename);

Dict * json_parse_string(char *string);

char *json_convert_dict_to_string(Dict *dict);

#endif