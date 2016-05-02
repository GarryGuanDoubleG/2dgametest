#include "audio.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include "simple_logger.h"

void audio_close();

void audio_init(int max_sounds, 
				int channels, 
				int channel_groups, 
				int max_music, 
				Bool enable_mp3,
				Bool enable_ogg)
{
	int flags = 0;

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		slog("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	atexit(Mix_CloseAudio);

	if(enable_mp3)
	{
		flags |= MIX_INIT_MP3;
	}
	if(enable_ogg)
	{
		flags != MIX_INIT_OGG;
	}
	if( !(Mix_Init(flags) & flags))
	{
		slog("Failed to initialize audio support: %s", SDL_GetError());
	}
	atexit(Mix_Quit);
	atexit(audio_close);

	sound_init(
		max_sounds,
		channels,
		channel_groups);
	music_init(max_music);
			
}


void audio_close()
{

}