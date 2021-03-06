
#pragma once

#include <cmath>

#ifndef M_PI
#include "graphicslib.h"
#define M_PI      ALLEGRO_PI
#endif

#ifndef WIN32
inline int min( int a, int b )
{
	return ( a <= b ? a : b );
};
#endif
