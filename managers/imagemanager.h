
#pragma once

#include "../framework/graphicslib.h"
#include <string>
#include <vector>

#ifndef Framework
class Framework;
#endif // Framework

struct PackedARGB8888
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

typedef struct ImageCache
{
	std::string* Path;
	double LastAccess;
	ALLEGRO_BITMAP* Image;
} ImageCache;

class ImageManager
{
  private:
    Framework* systemFramework;
		std::vector<ImageCache*> Cached;

		void RemoveCached( ImageCache* item );

		ALLEGRO_BITMAP* Scaler_PixelPerfect( ALLEGRO_BITMAP* Source, int ScaleSize );

  public:
		double CacheTime;

    ImageManager( Framework* FrameworkObject );
    ~ImageManager();

		ALLEGRO_BITMAP* GetImage(std::string Filename);
		ALLEGRO_BITMAP* GetImage(std::string Filename, bool CacheImage);
		ALLEGRO_BITMAP* GetGrayscaleImage(std::string Filename);
		ALLEGRO_BITMAP* GetNegativeImage(std::string Filename);
		ALLEGRO_BITMAP* GetScale2xImage(std::string Filename);
		ALLEGRO_BITMAP* GetScale3xImage(std::string Filename);
		ALLEGRO_BITMAP* GetScale4xImage(std::string Filename);

		void Clear();
		void Update();

};
