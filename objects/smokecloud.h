
#pragma once

#include "particle.h"

class SmokeCloud : public Particle
{

	public:
		SmokeCloud( Vector2* StartPosition, double StartVelocity, double StartAngle );

		virtual void Update();
		virtual void Render();

};