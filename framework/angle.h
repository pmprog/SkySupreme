
#pragma once

class Angle
{
	private:
		double curAngle;

	public:
		Angle( double StartAngle );
		void Add( double Addition );
		double ToRadians();

		bool ClockwiseShortestTo( double DestinationAngle );

};