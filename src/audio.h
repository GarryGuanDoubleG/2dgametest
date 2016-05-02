#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "types.h"
#include "dict.h"

/**
 * @brief initializes the audio system based
 * @param max_sounds the maximum number of sounds that can be allocated in memory at once
 * @param channels the number of audio channels to allocate into memory(separate from music channels)
 * @param channel_groups the number of channels to be reserved for groups to be set up
 * @param max_music the number of music files that can be played at the same time
 * @param enable_mp3 if true, inits the audio system to support mp3
*/

void audio_init(int max_sounds, int channels, int channel_groups, int max_music, Bool enable_mp3);

/**
* @brief initializes the audio system from a definition(config) file
* @param filename the filepath to the definition file
*/
void audio_init_from_def(char *filename);


#endif