
#include "audiomanager.h"
#include "../framework/framework.h"

AudioManager::AudioManager( Framework* FrameworkObject )
{
	SfxCacheTime = 240;
	MusicCacheTime = 600;
	systemFramework = FrameworkObject;
}

AudioManager::~AudioManager()
{
	SoundCache* fc;
	while( !SfxCached.empty() )
	{
		fc = SfxCached.back();
		SfxCached.pop_back();
		RemoveSfxCached( fc );
	}
	while( !MusicCached.empty() )
	{
		fc = MusicCached.back();
		MusicCached.pop_back();
		RemoveMusicCached( fc );
	}
}

ALLEGRO_SAMPLE* AudioManager::GetSoundFX(std::string Filename)
{
	SoundCache* fc;

	if( !SfxCached.empty() )
	{
		for( std::vector<SoundCache*>::iterator i = SfxCached.begin(); i != SfxCached.end(); i++ )
		{
			fc = (*i);
			if( fc->Path->compare( Filename ) == 0 )
			{
				fc->LastAccess = al_get_time();
				return (ALLEGRO_SAMPLE*)fc->Reference;
			}
		}
	}

#ifdef WRITE_LOG
  printf( "AudioManager: Loading SFX '%s'\n", Filename.c_str() );
#endif

	fc = (SoundCache*)malloc( sizeof(SoundCache) );
	fc->Path = new std::string(Filename);
	fc->Reference = al_load_sample( Filename.c_str() );
	fc->LastAccess = al_get_time();
	SfxCached.push_back( fc );

	return (ALLEGRO_SAMPLE*)fc->Reference;
}

ALLEGRO_AUDIO_STREAM* AudioManager::GetMusic(std::string Filename)
{
	SoundCache* fc;

	if( !MusicCached.empty() )
	{
		for( std::vector<SoundCache*>::iterator i = MusicCached.begin(); i != MusicCached.end(); i++ )
		{
			fc = (*i);
			if( fc->Path->compare( Filename ) == 0 )
			{
				fc->LastAccess = al_get_time();
				return (ALLEGRO_AUDIO_STREAM*)fc->Reference;
			}
		}
	}

#ifdef WRITE_LOG
  printf( "AudioManager: Loading Music '%s'\n", Filename.c_str() );
#endif

	fc = (SoundCache*)malloc( sizeof(SoundCache) );
	fc->Path = new std::string(Filename);
	fc->Reference = al_load_audio_stream( Filename.c_str(), 4, 2048 );
	fc->LastAccess = al_get_time();
	MusicCached.push_back( fc );

	return (ALLEGRO_AUDIO_STREAM*)fc->Reference;
}

void AudioManager::Update()
{
	SoundCache* fc;
	if( !SfxCached.empty() )
	{
		for( int i = SfxCached.size() - 1; i >= 0; i-- )
		{
			fc = SfxCached.at( i );
			if( al_get_time() - fc->LastAccess > SfxCacheTime )
			{
				SfxCached.erase( SfxCached.begin() + i );
				RemoveSfxCached( fc );
			}
		}
	}
	if( !MusicCached.empty() )
	{
    for( int i = MusicCached.size() - 1; i >= 0; i-- )
		{
			fc = MusicCached.at( i );
			if( al_get_time() - fc->LastAccess > MusicCacheTime )
			{
				MusicCached.erase( MusicCached.begin() + i );
				RemoveMusicCached( fc );
			}
		}
	}
}

void AudioManager::RemoveSfxCached( SoundCache* item )
{
#ifdef WRITE_LOG
  printf( "AudioManager: Unloading SFX '%s'\n", item->Path->c_str() );
#endif

	al_destroy_sample( (ALLEGRO_SAMPLE*)item->Reference );
	delete item->Path;
	free( (void*)item );
}

void AudioManager::RemoveMusicCached( SoundCache* item )
{
#ifdef WRITE_LOG
  printf( "AudioManager: Unloading Music '%s'\n", item->Path->c_str() );
#endif
  al_set_audio_stream_playing( (ALLEGRO_AUDIO_STREAM*)item->Reference, false );
	al_destroy_audio_stream( (ALLEGRO_AUDIO_STREAM*)item->Reference );
	delete item->Path;
	free( (void*)item );
}
