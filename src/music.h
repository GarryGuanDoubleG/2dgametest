#ifndef _MUSIC_H_
#define _MUSIC_H_
#include "types.h"

typedef struct Music_S Music;

/**
* @brief initializes the music system & sets up music queue
* @param max_music: maxinum number of music files that can play simultaneously
*/
void music_init(unsigned int max_music);

#endif