
#pragma once

#include "object.h"

#define STATE_FLYING			0
#define STATE_FLIPPING		1
#define STATE_STALLED			2
#define STATE_SHOOT				3
#define STATE_EXPLODING		4
#define STATE_EXPLODED		5

class Plane : public GameObject
{

	public:
		int Team;
		bool Flipped;
		int Health;
		int State;


		virtual void Update();
		virtual void Render();

};