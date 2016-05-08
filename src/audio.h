#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "types.h"
#include "dict.h"
#include <SDL_mixer.h>

typedef enum
{
	PLAYER_FX,
	ENEMY_FX,
	PROJECTILE_FX,
	SPELL_FX
}SGroup;


typedef struct Sound_S
{
	Mix_Chunk *sound;
	char filename[80];
	int used;
	float volume;
	int loop;
	int channel;
}Sound;

typedef struct Music_S
{
	Mix_Music *music;
	char filename[80];
	int used;
	float volume;
	int loop;
}Music;
/**
 * @brief initializes the audio system based
 * @param max_sounds the maximum number of sounds that can be allocated in memory at once
 * @param channels the number of audio channels to allocate into memory(separate from music channels)
 * @param channel_groups the number of channels to be reserved for groups to be set up
 * @param max_music the number of music files that can be played at the same time
 * @param enable_mp3 if true, inits the audio system to support mp3
*/

void audio_init(int max_sound, int max_music);
/**
* @brief initializes the audio system from a definition(config) file
* @param filename the filepath to the definition file
*/
void audio_init_from_def(char *filename);

void Sound_Free(Sound **sound);
void Music_Free(Music **music);

Sound *Sound_New(char *filename, int loop, int channel);
Music *Music_New(char *filename, int loop);

void Sound_Player(Sound *sound);
void Music_Player(Music *music);

#endif