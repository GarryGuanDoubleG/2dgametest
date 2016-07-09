#include "music.h"
#include "dict.h"
#include "load.h"
#include "simple_logger.h"
#include <glib.h>
#include <SDL.h>
#include <SDL_mixer.h>

static float __music_volume = 1;
static GList *__music_playlist = NULL;
static GList *__music_playlist_current = NULL;
static Bool   __music_playlist_play = Bool_False;
static int	  __music_max = -1;
struct Music_S
{
	Mix_Music *music;
	float volume;
	int loop;
};

void music_close();
void music_init(unsigned int max_music)
{
	__music_playlist = (GList*)(malloc(sizeof(GList)));
	__music_max = max_music;
	atexit(music_close);
}

void music_free(void * data)
{
	Music *music;
	if(!data) return;
	music = (Music*)data;
	Mix_FreeMusic(music->music);
}

void music_close()
{
	GList *iterator = NULL;
	Music *music;
	//music_stop_playlist();
	for(;iterator != NULL; iterator = iterator->next)
	{
		if(!iterator->data) continue;
		music = (Music *)iterator->data;
		music_free(&music);
		iterator->data = NULL;
	}
	g_list_free(__music_playlist);
	__music_playlist = NULL;
	__music_playlist_current = NULL;
}

Music *music_load_file(char *filename)
{
	Music *music = (Music*)malloc(sizeof(music));
	if(!filename)return NULL;
	music->music = Mix_LoadMUS(filename);
	if(music->music == NULL) 
	{
		slog("Error loading music %s: ",filename, SDL_GetError());
		return NULL;
	}
	music->volume = 1;
	music->loop = 1;
	g_list_append(__music_playlist, music);
	return music;
}

Bool music_load_resouce_all(char *filename)
{
	Music *music;
	Dict * music_hash_file;
	int i;
	Line key;
	if(!filename) return Bool_False;

	music_hash_file = Load_Dict_From_File("def/music.def");
	if(!music_hash_file)
	{
		return Bool_False;
	}
	if(music_hash_file->data_type != DICT_HASH)
	{
		return Bool_False;
	}

	for(i = 0; i < music_hash_file->item_count; i++)
	{
		music_load_file((char *)dict_get_hash_nth(key, music_hash_file, i)->keyValue);
	}
}