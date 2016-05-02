#include <jansson.h>
#include "json_parse.h"
#include "simple_logger.h"

Dict *json_convert(json_t *json);
//Dict *json_list_convert(json_t *json);
Dict *json_hash_convert(json_t *json);
json_t *json_from_dict(Dict *dict);

Dict *json_parse_file(char *filename)
{
  Dict *data;
  json_t *json;
  json_error_t jer;

  slog("begin json parse");
  json = json_load_file(filename,0,&jer);
  if (json == NULL)
  {
    slog("mgl_json_parse: failed to load file %s as json\n", filename);
    //mgl_json_log_error(&jer);
    return NULL;
  }
  data = json_convert(json);
  json_decref(json); 
  if (data == NULL)
  {
    //logger_debug("json_parse: failed to parse file %s as json\n", filename);
    //slog(&jer);
  }
  return data;
}

Dict *json_convert(json_t *json)
{
  if (json_is_object(json))
  {
    return json_hash_convert(json);
  }
/*  if (json_is_array(json))
  {
    return json_list_convert(json);
  }
  */
  if (json_is_string(json))
  {
    return dict_new_string((char *)json_string_value(json));
  }
  if (json_is_boolean(json))
  {
    return dict_new_bool(json_is_true(json));
  }
  if (json_is_integer(json))
  {
    return dict_new_int(json_integer_value(json));
  }
  if (json_is_real(json))
  {
    return dict_new_float(json_real_value(json));
  }
  if (json_is_null(json))
  {
    return NULL;
  }
  slog("failed to convert object\n");
  return NULL;
}
/*
Dict *mgl_json_list_convert(json_t *json)
{
  int index;
  json_t *value;
  Dict *data = Dict_new_list();
  json_array_foreach(json, index, value)
  {
    mgl_Dict_list_append(data,mgl_json_convert(value));
  }
  return data;
}
*/

json_t *json_from_hash(Dict *dict)
{
	json_t *json;
	int count, i;
	Line keystring;
	Dict *value;

	if(!dict) return NULL;
	json = json_object();
	if(!json) return NULL;
	count = dict_get_hash_count(dict);
	for(i = 0; i < count; i++)
	{
		value = dict_get_hash_nth(keystring, dict, i);
		if(!value) continue;
		json_object_set(json, keystring, json_from_dict(value));
	}
	return json;
}

Dict *json_hash_convert(json_t *json)
{
	Dict *data = dict_new_hash();	
	json_t *value;
	json_t *obj;
	const char *key;

	json_object_foreach(json, key, value)
	{
		slog("key: %s", key);
     	dict_hash_insert(data, key, json_convert(value));
	}
	
	return data;	
}

json_t * json_from_dict(Dict *dict)
{
	if(!dict) return NULL;
	switch(dict->data_type)
	{
		case DICT_HASH:
			return json_from_hash(dict);		
        /*case MGL_DICT_LIST:
            return mgl_json_from_list(dict);
            break;
			*/
        case DICT_INT:        
        case DICT_FLOAT:
        case DICT_STRING:
        case DICT_VOID:
        case DICT_CUSTOM:
            return json_string((char*)dict->keyValue);
            break;
    }
    return NULL;
}

char * json_convert_dict_to_string(Dict *dict)
{
	json_t *json;
    char *out;
    json = json_from_dict(dict);
    if (!json)return NULL;
    out = json_dumps(json, JSON_INDENT(4));
    json_decref(json);
    return out;
}