
#include "angle.h"

Angle::Angle( double StartAngle )
{
	curAngle = StartAngle;
}

void Add( double Addition )
{
	curAngle += Addition;
	if( curAngle >= 360.0 )
	{
		curAngle -= 360.0;
	}
	if( curAngle < 0.0 )
	{
		curAngle += 360.0;
	}
}

double ToRadians()
{
	return curAngle * (180 / M_PI);
}

bool ClockwiseShortestTo( double DestinationAngle )
{
	// TODO: Fix
	return true;
}
