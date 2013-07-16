
#pragma once

#include "../framework/graphicslib.h"
#include <vector>
#include <string>

#ifndef Framework
class Framework;
#endif // Framework

typedef struct SoundCache
{
	std::string* Path;
	double LastAccess;
	void* Reference;
} SoundCache;


class AudioManager
{
	private:
	  Framework* systemFramework;

		std::vector<SoundCache*> SfxCached;
		std::vector<SoundCache*> MusicCached;

		void RemoveSfxCached( SoundCache* item );
		void RemoveMusicCached( SoundCache* item );

	public:
		double SfxCacheTime;
		double MusicCacheTime;

		AudioManager( Framework* FrameworkObject );
		~AudioManager();

		ALLEGRO_SAMPLE* GetSoundFX(std::string Filename);
		ALLEGRO_AUDIO_STREAM* GetMusic(std::string Filename);

		void Update();
};
