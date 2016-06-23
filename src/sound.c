#include <SDL_mixer.h>
#include "dict.h"
#include "load.h"

#include "sound.h"
#include "simple_logger.h"



static int sound_channels = 0;
static int sound_reserv_channels = 0;
static int sound_group_channel = 0;
static int sound_group_id = 0;
static int sound_max = 0;
static Dict * sound_groups = NULL;
static Dict * sound_hash = NULL;//hash table with loaded sounds
struct Sound_S
{
	Mix_Chunk *chunk;
	float volume; /**<how loudly to play at*/
	unsigned int default_channel;/**<play sound on this channel by default */
};

void sound_close();
void sound_load_resources(char * filename);

void sound_init(int max_sounds, int channels, int channel_groups)
{
	Sound sound;

	sound_hash = Dict_New_Hash();
	sound_max = max_sounds;
	sound_groups = Dict_New_Hash();
	sound_channels = Mix_AllocateChannels(channels);
	sound_reserv_channels = Mix_ReserveChannels(channel_groups);
	atexit(sound_close);
}

void sound_free(void * data)
{
	Sound *sound;
	if(!data)return;
	sound = (Sound *)data;
	Mix_FreeChunk((sound->chunk));
}

void sound_close()
{
	int i;
	Line key;

	for( i = 0; i < sound_hash->item_count; i++)
	{
		sound_free(dict_get_hash_nth(key, sound_hash, i)->keyValue);
	}
	dict_free(&sound_groups);
}

Dict * sound_load_wav(char *filename)
{
	Dict * value = Dict_New();
	Sound * sound = (Sound *)malloc(sizeof(Sound));
	if(!filename) return NULL;
	sound->chunk = Mix_LoadWAV(filename);
	if(!sound->chunk)
	{
		slog("Error loading sound file %s", filename);
		return NULL;
	}
	sound->volume = 1;
	sound->default_channel = -1;
	
	value->keyValue = sound;
	value->item_count = 1;
	value->data_type = DICT_SOUND;
	value->keyFree = sound_free;
	return value;
}

void sound_load_all_resources(char *filename)
{
	int i = 0;
	Dict * sound_file_hash = load_dict_from_file("def/sound/sound_config.def");
	Line key;

	for( i = 0; i < sound_file_hash->item_count; i++)
	{
		Dict * value = dict_get_hash_nth(key, sound_file_hash, i);
		Dict_Hash_Insert(sound_hash, key, (Dict*)sound_load_wav((char *) sound_file_hash->keyValue));
	}
}

void sound_play(Sound *sound,
				float *volume,
				int   * channel,
				char  *group,
				int    loops)
{
	int chan;
	int id;
	float net_volume = 1,
		  g_volume = 1;
	if(!sound) return;
	if(volume)
	{
		net_volume = *volume;
	}

	if(group)
	{

		net_volume *= g_volume;
	}
	else if(channel)
	{
		chan = *channel;
	}
	else
	{
		chan = sound->default_channel;
	}

	Mix_VolumeChunk(sound->chunk, (int)(net_volume * MIX_MAX_VOLUME));
	Mix_PlayChannel(chan, sound->chunk, loops);
}

