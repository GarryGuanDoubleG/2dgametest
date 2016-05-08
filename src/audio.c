#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"

int g_music_max = 0;
int g_sound_max = 0;
int numMusics = 0;

int g_sound_nums = 0;

Sound *soundList = NULL;
Music *musicList = NULL;

void audio_close_all();

void audio_init(int max_sound, int max_music)				
{
	int flags = MIX_INIT_OGG | MIX_INIT_MP3;

	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
	{
		slog("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
	if(Mix_AllocateChannels(5) != 5)return;
	if((Mix_Init(flags) !=  flags))
	{
		slog("Failed to initialize audio support: %s", SDL_GetError());
		return;
	}

	g_sound_max = max_sound;
	g_music_max = max_music;

	musicList = (Music *) malloc (sizeof (Music) * g_music_max);
	soundList = (Sound *) malloc (sizeof (Sound) * g_sound_max);

	memset(musicList, 0, sizeof (Music) * g_music_max);
	memset(soundList, 0, sizeof (Sound) * g_sound_max);

	atexit(Mix_CloseAudio);
	atexit(Mix_Quit);
	atexit(audio_close_all);			
}

void Sound_Free(Sound **sound)
{
	if(!sound)return;
	else if(!*sound)return;

	(*sound)->used--;

	if((*sound)->sound != NULL)
	{
		Mix_FreeChunk((*sound)->sound);
	}
	g_sound_nums--;
	*sound = NULL;
}

void Music_Free(Music **music)
{
	if(!music)return;
	else if(!*music)return;

	(*music)->used--;

	if((*music)->music != NULL)
	{
		Mix_FreeMusic((*music)->music);
	}
	numMusics--;
	*music = NULL;
}

void Sound_Close()
{
	int i;
	Sound *sound;

	if(!soundList)return;

	for(i = 0; i < g_sound_nums; i++)
	{
		sound = &soundList[i];
		Sound_Free(&sound);
	}
	g_sound_nums = 0;
	free(soundList); 
	soundList = NULL;
}

void Music_Close()
{
	int i;
	Music *music;

	if(!musicList)return;

	for(i = 0; i < g_music_max; i++)
	{
		music = &musicList[i];
		Music_Free(&music);
	}
	g_music_max = 0;
	free(musicList); 
	musicList = NULL;
}


void audio_close_all()
{	
	Sound_Close();
	Music_Close();
	Mix_CloseAudio();
}

Sound *Sound_New(char *filename, int loop, int channel)
{
	int i;

	for(i = 0; i < g_sound_nums; i++)
	{
		if(soundList[i].used)
		{
			continue;
		}
		memset(&soundList[i],0,sizeof(Sound));		
		soundList[i].used = 1;
		g_sound_nums++;
		
		if(g_sound_nums > g_sound_nums)
		{
			slog("Maximum sounds reached");
			exit(1);
		}

		strcpy(soundList[i].filename, filename);
		soundList[i].channel = channel;
		soundList[i].loop = loop;
		soundList[i].sound = Mix_LoadWAV(filename);

		return &soundList[i];
	}
	return NULL;
}

Music *Music_New(char *filename, int loop)
{
	int i;

	for(i = 0; i < g_music_max; i++)
	{
		if(musicList[i].used)
		{
			continue;
		}
		memset(&musicList[i],0,sizeof(Music));
		musicList[i].used = 1;
		numMusics++;

		if(numMusics > g_music_max)
		{
			slog("Maximum musics reached");
			exit(1);
		}

		strcpy(musicList[i].filename, filename);
		musicList[i].music = Mix_LoadMUS(filename);
		musicList[i].loop = loop;
		return &musicList[i];
	}
	return NULL;
}

void Sound_Player(Sound *sound)
{
	Mix_PlayChannel(sound->channel, sound->sound, sound->loop);
}

void Music_Player(Music *music)
{
	Mix_PlayMusic(music->music, music->loop);
}