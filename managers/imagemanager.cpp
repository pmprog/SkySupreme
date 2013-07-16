
#include "imagemanager.h"
#include "../framework/framework.h"

ImageManager::ImageManager( Framework* FrameworkObject )
{
	CacheTime = 600;
	systemFramework = FrameworkObject;
}

ImageManager::~ImageManager()
{
	Clear();
}

ALLEGRO_BITMAP* ImageManager::GetImage(std::string Filename)
{
  return GetImage( Filename, true );
}

ALLEGRO_BITMAP* ImageManager::GetImage(std::string Filename, bool CacheImage)
{
	ImageCache* fc;

	if( !Cached.empty() )
	{
		for( std::vector<ImageCache*>::iterator i = Cached.begin(); i != Cached.end(); i++ )
		{
			fc = (*i);
			if( fc->Path->compare( Filename ) == 0 )
			{
				fc->LastAccess = al_get_time();
				return fc->Image;
			}
		}
	}

#ifdef WRITE_LOG
  printf( "ImageManager: Loading Image '%s'\n", Filename.c_str() );
#endif

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(Filename);
	fc->Image = al_load_bitmap( Filename.c_str() );
	fc->LastAccess = al_get_time() - ( CacheImage ? 0 : CacheTime );

	if( fc->Image == 0 )
	{
		delete fc;
		return 0;
	}

	Cached.push_back( fc );

	return fc->Image;
}

void ImageManager::Update()
{
	ImageCache* fc;
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

void ImageManager::Clear()
{
	ImageCache* fc;
	while( !Cached.empty() )
	{
		fc = Cached.front();
		Cached.erase( Cached.begin() );
		RemoveCached( fc );
	}
}


void ImageManager::RemoveCached( ImageCache* item )
{
#ifdef WRITE_LOG
  printf( "ImageManager: Unloading Image '%s'\n", item->Path->c_str() );
#endif

	al_destroy_bitmap( item->Image );
	delete item->Path;
	free( (void*)item );
}

ALLEGRO_BITMAP* ImageManager::GetGrayscaleImage( std::string Filename )
{
	ALLEGRO_BITMAP* gsImg;
	PackedARGB8888* gsCol;
	ImageCache* fc;

	std::string EncodedFilename;
	EncodedFilename.clear();
	EncodedFilename.append( Filename );
	EncodedFilename.append( ":GREY" );

#ifdef WRITE_LOG
  printf( "ImageManager: Loading Greyscaled Image '%s'\n", Filename.c_str() );
#endif

	gsImg = GetImage( EncodedFilename );
	if( gsImg != 0 )
		return gsImg;

	gsImg = GetImage( Filename );
	if( gsImg == 0 )
		return 0;

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(EncodedFilename);
	fc->Image = al_load_bitmap( Filename.c_str() );
	fc->LastAccess = al_get_time();

	int imgW = al_get_bitmap_width( fc->Image );
	int imgH = al_get_bitmap_height( fc->Image );

	ALLEGRO_LOCKED_REGION* rgn = al_lock_bitmap( fc->Image, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READWRITE );

	for( int y = 0; y < imgH; y++ )
	{
		for( int x = 0; x < imgW; x++ )
		{
			gsCol = &((PackedARGB8888*)rgn->data)[(y * imgW) + x];
			gsCol->r = (unsigned char)(((float)gsCol->r * 0.35) + ((float)gsCol->g * 0.5) + ((float)gsCol->b * 0.15));
			gsCol->g = gsCol->r;
			gsCol->b = gsCol->r;

		}
	}

	al_unlock_bitmap( fc->Image );

	Cached.push_back( fc );

	return fc->Image;
}

ALLEGRO_BITMAP* ImageManager::GetNegativeImage(std::string Filename)
{
	ALLEGRO_BITMAP* gsImg;
	PackedARGB8888* gsCol;
	ImageCache* fc;

	std::string EncodedFilename;
	EncodedFilename.clear();
	EncodedFilename.append( Filename );
	EncodedFilename.append( ":NEG" );

#ifdef WRITE_LOG
  printf( "ImageManager: Loading Negative Image '%s'\n", Filename.c_str() );
#endif

	gsImg = GetImage( EncodedFilename );
	if( gsImg != 0 )
		return gsImg;

	gsImg = GetImage( Filename );
	if( gsImg == 0 )
		return 0;

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(EncodedFilename);
	fc->Image = al_load_bitmap( Filename.c_str() );
	fc->LastAccess = al_get_time();

	int imgW = al_get_bitmap_width( fc->Image );
	int imgH = al_get_bitmap_height( fc->Image );

	ALLEGRO_LOCKED_REGION* rgn = al_lock_bitmap( fc->Image, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READWRITE );

	for( int y = 0; y < imgH; y++ )
	{
		for( int x = 0; x < imgW; x++ )
		{
			gsCol = &((PackedARGB8888*)rgn->data)[(y * imgW) + x];
			gsCol->r = (unsigned char)(255 - gsCol->r);
			gsCol->g = (unsigned char)(255 - gsCol->g);
			gsCol->b = (unsigned char)(255 - gsCol->b);
		}
	}

	al_unlock_bitmap( fc->Image );

	Cached.push_back( fc );

	return fc->Image;
}

ALLEGRO_BITMAP* ImageManager::GetScale2xImage(std::string Filename)
{
	ImageCache* fc;
	ALLEGRO_BITMAP* source = GetImage( Filename );
	ALLEGRO_BITMAP* scaled;
	ALLEGRO_BITMAP* tmpTarget = al_get_target_bitmap();

	std::string EncodedFilename;
	EncodedFilename.clear();
	EncodedFilename.append( Filename );
	EncodedFilename.append( ":x2" );

#ifdef WRITE_LOG
  printf( "ImageManager: Loading x2 Image '%s'\n", Filename.c_str() );
#endif

	scaled = GetImage( EncodedFilename );
	if( scaled != 0 )
		return scaled;

	scaled = Scaler_PixelPerfect( source, 2 );

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(Filename);
	fc->Path->append(":x2");
	fc->Image = scaled;
	fc->LastAccess = al_get_time() - CacheTime;

	if( fc->Image == 0 )
	{
		delete fc;
		return 0;
	}

	Cached.push_back( fc );

	return fc->Image;
}

ALLEGRO_BITMAP* ImageManager::GetScale3xImage(std::string Filename)
{
	ImageCache* fc;
	ALLEGRO_BITMAP* source = GetImage( Filename );
	ALLEGRO_BITMAP* scaled;
	ALLEGRO_BITMAP* tmpTarget = al_get_target_bitmap();

	std::string EncodedFilename;
	EncodedFilename.clear();
	EncodedFilename.append( Filename );
	EncodedFilename.append( ":x3" );

#ifdef WRITE_LOG
  printf( "ImageManager: Loading x3 Image '%s'\n", Filename.c_str() );
#endif

	scaled = GetImage( EncodedFilename );
	if( scaled != 0 )
		return scaled;

	scaled = Scaler_PixelPerfect( source, 3 );

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(Filename);
	fc->Path->append(":x3");
	fc->Image = scaled;
	fc->LastAccess = al_get_time() - CacheTime;

	if( fc->Image == 0 )
	{
		delete fc;
		return 0;
	}

	Cached.push_back( fc );

	return fc->Image;
}

ALLEGRO_BITMAP* ImageManager::GetScale4xImage(std::string Filename)
{
	ImageCache* fc;
	ALLEGRO_BITMAP* source = GetImage( Filename );
	ALLEGRO_BITMAP* scaled;
	ALLEGRO_BITMAP* tmpTarget = al_get_target_bitmap();

	std::string EncodedFilename;
	EncodedFilename.clear();
	EncodedFilename.append( Filename );
	EncodedFilename.append( ":x4" );

#ifdef WRITE_LOG
  printf( "ImageManager: Loading x4 Image '%s'\n", Filename.c_str() );
#endif

	scaled = GetImage( EncodedFilename );
	if( scaled != 0 )
		return scaled;

	scaled = Scaler_PixelPerfect( source, 4 );

	fc = (ImageCache*)malloc( sizeof(ImageCache) );
	fc->Path = new std::string(Filename);
	fc->Path->append(":x4");
	fc->Image = scaled;
	fc->LastAccess = al_get_time() - CacheTime;

	if( fc->Image == 0 )
	{
		delete fc;
		return 0;
	}

	Cached.push_back( fc );

	return fc->Image;
}

ALLEGRO_BITMAP* ImageManager::Scaler_PixelPerfect( ALLEGRO_BITMAP* Source, int ScaleSize )
{
	uint32_t pixelData;
	int imgW = al_get_bitmap_width( Source );
	int imgH = al_get_bitmap_height( Source );

	ALLEGRO_BITMAP* Scaled = al_create_bitmap( al_get_bitmap_width( Source ) * ScaleSize, al_get_bitmap_height( Source ) * ScaleSize );

	ALLEGRO_LOCKED_REGION* rgnSrc = al_lock_bitmap( Source, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READONLY );
	ALLEGRO_LOCKED_REGION* rgnDst = al_lock_bitmap( Scaled, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READWRITE );

	for( int y = 0; y < imgH; y++ )
	{
		for( int yM = 0; yM < ScaleSize; yM++ )
		{
			int DestY = (y * ScaleSize) + yM;

			for( int x = 0; x < imgW; x++ )
			{
				pixelData = ((uint32_t*)rgnSrc->data)[(y * imgW) + x];
				for( int xM = 0; xM < ScaleSize; xM++ )
				{
					((uint32_t*)rgnDst->data)[ (DestY * (imgW * ScaleSize)) + (x * ScaleSize) + xM] = pixelData;
				}
			}
		}
	}

	al_unlock_bitmap( Scaled );
	al_unlock_bitmap( Source );

	return Scaled;
}