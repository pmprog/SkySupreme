
#include "fontmanager.h"
#include "../framework/framework.h"

FontManager::FontManager( Framework* FrameworkObject )
{
	CacheTime = 120;
	systemFramework = FrameworkObject;
}

FontManager::~FontManager()
{
	FontCache* fc;
	while( !Cached.empty() )
	{
		fc = Cached.back();
		Cached.pop_back();
		RemoveCached( fc );
	}
}

ALLEGRO_FONT* FontManager::GetFont(std::string Filename, int Size, int Flags)
{
	FontCache* fc;

	if( !Cached.empty() )
	{
		for( std::vector<FontCache*>::iterator i = Cached.begin(); i != Cached.end(); i++ )
		{
			fc = (*i);
			if( fc->Path->compare( Filename ) == 0 && fc->Size == Size && fc->Flags == Flags )
			{
				fc->LastAccess = al_get_time();
				return fc->Font;
			}
		}
	}

#ifdef WRITE_LOG
  printf( "FontManager: Loading Font '%s' with size %d\n", Filename.c_str(), Size );
#endif

	fc = (FontCache*)malloc( sizeof(FontCache) );
	fc->Path = new std::string(Filename);
	fc->Size = Size;
	fc->Flags = Flags;
	fc->Font = al_load_font( Filename.c_str(), Size, Flags );
	fc->LastAccess = al_get_time();
	Cached.push_back( fc );

	return fc->Font;
}

void FontManager::Update()
{
	FontCache* fc;
	if( !Cached.empty() )
	{
		for( int i = Cached.size() - 1; i >= 0; i-- )
		{
			fc = Cached.at( i );
			if( al_get_time() - fc->LastAccess > CacheTime )
			{
				Cached.erase( Cached.begin() + i );
				RemoveCached( fc );
			}
		}
	}
}

void FontManager::Clear()
{
	FontCache* fc;
	while( !Cached.empty() )
	{
		fc = Cached.front();
		Cached.erase( Cached.begin() );
		RemoveCached( fc );
	}
}

void FontManager::RemoveCached( FontCache* item )
{
#ifdef WRITE_LOG
  printf( "FontManager: Unloading Font '%s' with size %d\n", item->Path->c_str(), item->Size );
#endif

	al_destroy_font( item->Font );
	delete item->Path;
	free( (void*)item );
}
