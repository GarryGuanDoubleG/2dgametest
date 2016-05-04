#ifndef _SOUND_H_
#define _SOUND_H_
#include "types.h"

/**
* @brief initializes the sound system based on parameters
* @param max_sounds: maximum number of sounds that can be loaded at the same time
* @param channels number of audio channels exluding music
* @param channel_groups the number of channelsfor groups to be set up
*/
typedef struct Sound_S Sound;

void sound_init(int max_sounds, int channels, int channel_groups);

/**
* @brief frees memory allocated from sound system
*/
void sound_close();
/**
* @brief loads a playable sound file into a dictionary
* @param filename of wav file to load
* @return dictionary containing sound file
*/
Dict * sound_load_wav(char *filename);

void sound_play(Sound *sound,
				float *volume,
				int   * channel,
				char  *group,
				int    loops);
#endif