
#pragma once

#include "particle.h"

class Explosion : public Particle
{

	public:
		Explosion( Vector2* StartPosition );

		virtual void Update();
		virtual void Render();

};