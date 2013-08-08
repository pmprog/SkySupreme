
#pragma once

class FwAngle
{
	private:
		double curAngle;

	public:
		FwAngle( double StartAngle );
		void Add( double Degrees );
		double ToDegrees();
		double ToRadians();

		bool ClockwiseShortestTo( FwAngle* DestinationAngle );
		bool ClockwiseShortestTo( double DestinationAngle );
		void RotateShortestBy( FwAngle* DestinationAngle, double ByDegrees );
		void RotateShortestBy( double DestinationAngle, double ByDegrees );

};