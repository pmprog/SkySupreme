
#pragma once

#include "particle.h"

class Cloud : public Particle
{
	private:
		int CloudStyle;
		int TileMultiplier;

	public:
		Cloud( Vector2* StartPosition, double StartVelocity, double StartAngle );

		virtual void Update();
		virtual void Render();

		void SetTileScale( int ScreenMultiplier );

};