
#include "angle.h"
#include "../framework/vector2.h"

FwAngle::FwAngle( double StartAngle )
{
	curAngle = StartAngle;
}

void FwAngle::Add( double Degrees )
{
	curAngle += Degrees;
	while( curAngle >= 360.0 )
	{
		curAngle -= 360.0;
	}
	while( curAngle < 0.0 )
	{
		curAngle += 360.0;
	}
}

double FwAngle::ToDegrees()
{
	return curAngle;
}

double FwAngle::ToRadians()
{
	return curAngle * (180.0 / M_PI);
}

bool FwAngle::ClockwiseShortestTo( FwAngle* DestinationAngle )
{
	return ClockwiseShortestTo( DestinationAngle->ToDegrees() );
}

bool FwAngle::ClockwiseShortestTo( double DestinationAngle )
{
	double diff = DestinationAngle - curAngle;
	while( diff >= 360.0 )
	{
		diff -= 360.0;
	}
	while( diff < 0.0 )
	{
		diff += 360.0;
	}
	return (diff < 180.0);
}

void FwAngle::RotateShortestBy( FwAngle* DestinationAngle, double ByDegrees )
{
	return RotateShortestBy( DestinationAngle->ToDegrees(), ByDegrees );
}


void FwAngle::RotateShortestBy( double DestinationAngle, double ByDegrees )
{
	if( ClockwiseShortestTo( DestinationAngle ) )
	{
		Add( ByDegrees );
	} else {
		Add( -ByDegrees );
	}
}
