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



#endif