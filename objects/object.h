
#pragma once

#include "../framework/vector2.h"
#include "../framework/framework.h"

#ifndef GameStage
class GameStage;
#endif

class GameObject
{
	protected:
		int Animation_Delay;

	public:
		static GameStage* Game;

		int Zorder;

		GameObject* Owner;

		int Animation_CurrentFrame;
		int Animation_TicksPerFrame;

		Vector2* Position;
		double Velocity;
		double Angle;

		bool ForRemoval;			// The object is finished, remove it from the game

		GameObject( double StartX, double StartY );
		GameObject( Vector2* StartPosition );

		virtual void Update();
		virtual void Render() = 0;

		static bool CompareZ( GameObject* first, GameObject* second );

};

