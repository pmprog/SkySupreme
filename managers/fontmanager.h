
#pragma once

#include "../framework/graphicslib.h"
#include <vector>
#include <string>

#ifndef Framework
class Framework;
#endif // Framework

typedef struct FontCache
{
	std::string* Path;
	int Size;
	int Flags;
	double LastAccess;
	ALLEGRO_FONT* Font;
} FontCache;


class FontManager
{
	private:
	  Framework* systemFramework;
		std::vector<FontCache*> Cached;

		void RemoveCached( FontCache* item );

	public:
		double CacheTime;

		FontManager( Framework* FrameworkObject );
		~FontManager();

		ALLEGRO_FONT* GetFont(std::string Filename, int Size, int Flags);

		void Update();
		void Clear();
};
