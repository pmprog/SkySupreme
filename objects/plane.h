
#pragma once

#include "object.h"

#define STATE_FLYING			0
#define STATE_FLIPPING		1
#define STATE_STALLED			2
#define STATE_SHOOT				3
#define STATE_HIT					4
#define STATE_EXPLODING		5
#define STATE_EXPLODED		6

#define PLANE_VELOCITY_MAX				20.0
#define PLANE_VELOCITY_STALLEXIT	10.0

#define PLANE_DAMAGE_MAX					8
#define PLANE_DAMAGE_SMOKE_SMALL	3
#define PLANE_DAMAGE_SMOKE_LOTS		1

class Plane : public GameObject
{
	private:
		bool RotateLeft;
		bool RotateRight;
		bool HasShot;
		int LastSmokeFrame;

		void SetState( int NewState );

	public:
		int Team;
		bool Flipped;
		int Health;
		int State;

		// Individual aids, but can turn all on for AI mode :)
		bool Controller_Assistance_AutoFlip;
		bool Controller_Assistance_AutoFire;
		bool Controller_Assistance_AutoFly;
		bool Controller_Keyboard;
		ALLEGRO_JOYSTICK* Controller_Joystick;

		Plane( ALLEGRO_JOYSTICK* Controller, Vector2* StartPosition, double StartVelocity, double StartAngle );

		void Event( FwEvent* e );
		virtual void Update();
		virtual void Render();

		void ProcessFlyingAI();
};