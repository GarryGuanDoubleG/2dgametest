#include <stdio.h>
#include <string.h>
#include <glib.h>
/*#include <glib/gstring.h>
#include <glib/glist.h>
#include <glib/ghash.h>
*/
#include "simple_logger.h"
#include "dict.h"

void Dict_Free_Value(gpointer *val)
{
	if(!val) return;
	g_free(val);
}

void dict_g_string_free(char * string)
{
	if(!string) return;
	g_free(string);
}

void dict_destroy(Dict * dict)
{
	dict_free(&dict);
}

void dict_free(Dict ** dict){ 
	if(!dict) return;
	if(!*dict) return;
	if((*dict)->keyValue != NULL)
	{
		if((*dict)->keyFree != NULL)
		{
			(*dict)->keyFree((*dict)->keyValue);
		}
		else
		{
			free((*dict)->keyValue);
		}
	}
	free (*dict);
	dict = NULL;
}

void dict_hash_free(Dict * dict_hash)
{
  if (!dict_hash)return;
  if (dict_hash->data_type!= DICT_HASH)return;
  g_hash_table_destroy((GHashTable*)dict_hash->keyValue);
  free(dict_hash);
}

Dict * Dict_New()
{
	Dict *new_dict;
	new_dict = (Dict*) malloc(sizeof(new_dict));
	if(new_dict == NULL)
	{
		return NULL;
	}
	memset(new_dict, 0, sizeof(new_dict));
	return new_dict;
}

Dict * Dict_New_Hash()
{
	Dict *new_dict;
	new_dict = Dict_New();
	if(!new_dict) return NULL;
	new_dict->data_type = DICT_HASH;
	new_dict->item_count = 0;
	new_dict->keyFree = (Dict_Free)dict_hash_free;
	new_dict->keyValue = 
		g_hash_table_new_full(g_str_hash,
							  g_str_equal,
							  (GDestroyNotify)dict_g_string_free,
							  (GDestroyNotify)dict_destroy);
	return new_dict;
}

Dict *Dict_New_Array(int data_size)
{
	Dict *new_dict;
	new_dict = Dict_New();
	if(!new_dict) return NULL;
	new_dict->data_type = DICT_ARRAY;
	new_dict->item_count = 0;
	new_dict->keyFree = (Dict_Free)dict_hash_free;
	new_dict->keyValue = 
		g_array_new(Bool_False, Bool_False, data_size);
	return new_dict;
}

Dict *Dict_New_bool(Bool n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%s", string_from_bool(n));
  return Dict_New_String(text);
}

Dict *Dict_New_Int(int n)
{
	Dict *new_dict; 
	//should probably make a memory pool system instead of using malloc a million times
	int *temp = (int*)(malloc(sizeof(int)));
	new_dict = Dict_New();
  
	if (!new_dict)return NULL;

	*temp = n;

	new_dict->data_type = DICT_INT;
	new_dict->item_count = 1;
	new_dict->keyFree = (Dict_Free)Dict_Free_Value;
	//new_dict->keyClone = dict_clone_string;
	new_dict->keyValue = (void*)temp;
	return new_dict;
}

Dict *Dict_New_uint(unsigned int n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%u",n);
  return Dict_New_String(text);
}

Dict *Dict_New_float(float n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%f",n);
  return Dict_New_String(text);
}

Dict *Dict_New_vec2d(Vec2d n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%f,%f",n.x,n.y);
  return Dict_New_String(text);
}

Dict *Dict_New_vec3d(Vec3d n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%f,%f,%f",n.x,n.y,n.z);
  return Dict_New_String(text);
}

Dict *Dict_New_rect(SDL_Rect n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%i,%i,%i,%i",n.x,n.y,n.w,n.h);
  return Dict_New_String(text);
}

Dict *Dict_New_rectf(Rect_f n)
{
  Line text;
  sprintf_s(text,LINE_LENGTH,"%f,%f,%f,%f",n.x,n.y,n.w,n.h);
  return Dict_New_String(text);
}

Dict *Dict_New_String(char *text)
{
  Dict *new_dict; 
  new_dict = Dict_New();
  if (!new_dict)return NULL;
  new_dict->data_type = DICT_STRING;
  new_dict->item_count = strlen(text);
  new_dict->keyFree = (Dict_Free)dict_g_string_free;
  //new_dict->keyClone = dict_clone_string;
  new_dict->keyValue = g_strdup(text);
  return new_dict;
}

void Dict_Hash_Insert(Dict *hash, const char *key, gpointer value)
{
	GHashTable *hash_table = NULL;
	if(!hash) return;
	if(hash->data_type != DICT_HASH) return;
	if(hash->keyValue == NULL) return;
	hash_table = (GHashTable*) hash->keyValue;	
	if(g_hash_table_lookup(hash_table, key) != NULL)
	{
		g_hash_table_replace(hash_table, g_strdup(key), value);
	}
	else
	{
		g_hash_table_insert(hash_table, g_strdup(key), value);
		hash->item_count++;
	}
}

void dict_hash_remove(Dict*hash, char *key)
{
	GHashTable *hash_table = NULL;
	if (!hash)return;
	if (hash->data_type != DICT_HASH)return;
	if (hash->keyValue == NULL)return;
	hash_table = (GHashTable*)hash->keyValue;
	g_hash_table_remove(hash_table,key);
}

Dict * Dict_Get_Hash_Value(Dict*hash, Line key)
{
	GHashTable *hash_table = NULL;
	gpointer value = NULL;
	if(!hash) return NULL;
	if(hash->data_type != DICT_HASH) return NULL;
	if(hash->keyValue == NULL)
	{
		return NULL;
	}

	hash_table = (GHashTable*)hash->keyValue;
	return (Dict *)g_hash_table_lookup(hash_table, key);
}

int dict_get_hash_count(Dict * hash)
{
	if(!hash) return 0;
	if(hash->data_type != DICT_HASH) return 0;
	if(hash->keyValue == NULL) return 0;
	return hash->item_count; 
}

Dict* dict_get_hash_nth(Line key, Dict* hash, int n)
{
	GHashTable *hash_table = NULL;
	Dict*result = NULL;
	GList *keys = NULL;

	if(!hash) return NULL;
	if(hash->data_type != DICT_HASH) return NULL;
	if(hash->keyValue == NULL) return NULL;

	hash_table = (GHashTable*)hash->keyValue;
	keys = g_hash_table_get_keys(hash_table);
	if(!keys) return NULL;
	strncpy(key, (char*)g_list_nth_data(keys, n), LINE_LENGTH);
	result = Dict_Get_Hash_Value(hash, key);
	g_list_free(keys);
	return result;
}

void Dict_Array_Cpy(Dict * dict_array, int index, void * data, int array_len)
{
	if(!dict_array) return;
	if(dict_array->data_type != DICT_ARRAY) return;
	if(!data) return;
	if(index < 0)
	{
		slog("DICT_ARRAY: Index cannot be < 0");
		return;
	}

	dict_array->keyValue = g_array_insert_vals((GArray*)dict_array->keyValue, index, data, array_len);
	dict_array->item_count = array_len;
}
/*
void Dict_Array_Append(Dict *dict)
{
  if (!dict)return;
  if (dict->keyType != DICT_ARRAY)return;
  list->keyValue = g_array_append_(list->keyValue,item);
  list->itemCount++;
}

*/