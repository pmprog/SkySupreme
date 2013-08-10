
#pragma once

#include <cmath>

#ifndef M_PI
#include "graphicslib.h"
#define M_PI      ALLEGRO_PI
#endif

#ifndef abs
inline int abs( int a )
{
	return ( a < 0 ? a * -1 : a );
};
#endif

#ifndef min
inline int min( int a, int b )
{
	return ( a <= b ? a : b );
};
#endif
