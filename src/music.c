#include "music.h"
#include "dict.h"
#include "load.h"
#include <glib/glist.h>
#include <glib.h>
#include <SDL.h>
#include <SDL_mixer.h>

static float music_volume = 1;
static GList *music_playlist = NULL;
static GList *music_playlist_current = NULL;
static Bool   music_playlist_play = Bool_False;

struct Music_S
{
	Mix_Music *music;
	float volume;
	int loop;
};

void music_close();
void music_init(unsigned int max_music)
{
	music_playlist = (GList*)(malloc(sizeof(GList));
	atexit(music_close);
}

Music *music_load_file(char *filename)
{
	Music *music;

	if(!filename)
	{
		music->music = Mix_LoadMUS(filename);
	}
	g_list_append(music_playlist, music);
}

Bool music_load_resouce_all(char *filename)
{
	Music *music;
	Dict * music_hash_file;
	int i;
	Line key;
	if(!filename) return Bool_False;

	music_hash_file = load_dict_from_file("def/music.def");
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