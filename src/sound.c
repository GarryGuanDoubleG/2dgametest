#include "sound.h"
#include "simple_logger.h"
#include "dict.h"
#include <SDL_mixer.h>

static int sound_channels = 0;
static int sound_reserv_channels = 0;
static int sound_group_channel = 0;
static int sound_group_id = 0;
static Dict * sound_groups = NULL;

struct Sound_S
{
	Mix_Chunk *chunk;
	float volume; /**<how loudly to play at*/
	unsigned int default_channel;/**<play sound on this channel by default */
};

void sound_close();


void sound_init(int max_sounds, int channels, int channel_groups)
{
	Sound sound;

}
