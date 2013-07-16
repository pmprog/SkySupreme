
#pragma once

#include "particle.h"

class Cloud : public Particle
{

	public:
		Cloud( Vector2* StartPosition );

		virtual void Update();
		virtual void Render();

};