#include <jansson.h>
#include "json_parse.h"
#include "simple_logger.h"

Dict *json_convert(json_t *json);
//Dict *json_list_convert(json_t *json);
Dict *json_hash_convert(json_t *json);
json_t *json_from_dict(Dict *dict);
Dict *json_array_convert(json_t *json);

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

  if (json_is_array(json))
  {
    return json_array_convert(json);
  }
  
  if (json_is_string(json))
  {
    return Dict_New_String((char *)json_string_value(json));
  }
  if (json_is_boolean(json))
  {
    return Dict_New_bool(json_is_true(json));
  }
  if (json_is_integer(json))
  {
    return Dict_New_Int(json_integer_value(json));
  }
  if (json_is_real(json))
  {
    return Dict_New_float(json_real_value(json));
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
  Dict *data = Dict_New_list();
  json_array_foreach(json, index, value)
  {
    mgl_Dict_list_append(data,mgl_json_convertvalue));
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

json_t *json_from_array(Dict *dict)		
{
	json_t *json;
	int count, i;
	int value;
	GArray *g_array;

	if(!dict) return NULL;
	if(dict->data_type != DICT_ARRAY) return NULL;

	g_array = (GArray*)dict->keyValue;

	json = json_array();
	if(!json) return NULL;

	count = dict->item_count;

	for(i = 0; i < count; i++)
	{
		//value = dict_get_hash_nth(keystring, dict, i);
		//if(!value) continue;
		value = g_array_index(g_array, int, i);

		json_array_append_new(json, json_integer(value));
	}
	return json;
}

Dict *json_hash_convert(json_t *json)
{
	Dict *data = Dict_New_Hash();	
	json_t *value;
	json_t *obj;
	const char *key;

	json_object_foreach(json, key, value)
	{
		slog("key: %s", key);
     	Dict_Hash_Insert(data, key, json_convert(value));
	}
	
	return data;	
}

Dict *json_array_convert(json_t *json)
{
  int index = 0;
  int size;
  json_t *value;
  int *convert_array;
  Dict *data;

  data = Dict_New_Array( sizeof(int) );

  if(!data)return NULL;
  data->data_type = DICT_ARRAY;

  size = json_array_size( json );
  if(size == NULL) return NULL;

  data->item_count = size;

  convert_array = (int *)malloc(sizeof(int) * size);
  data->keyValue = (void *)convert_array;

  json_array_foreach(json, index, value)
  {
	  int temp = json_integer_value(value);
	  convert_array[index] = temp;
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
		case DICT_ARRAY:
			return json_from_array(dict);
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