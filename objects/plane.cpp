
#include "plane.h"
#include "../stages/game.h"
#include "bullet.h"
#include "smokecloud.h"

Plane::Plane( ALLEGRO_JOYSTICK* Controller, Vector2* StartPosition, double StartVelocity, double StartAngle ) : GameObject( StartPosition )
{
	Velocity = StartVelocity;
	Angle = StartAngle;

	LastSmokeFrame = 0;
	Team = 0;
	Flipped = false;
	Health = PLANE_DAMAGE_MAX;
	State = STATE_FLYING;

	RotateLeft = false;
	RotateRight = false;

	Controller_Assistance_AutoFlip = false;
	Controller_Assistance_AutoFire = false;
	Controller_Assistance_AutoFly = false;

	if( Controller == 0 )
	{
		Controller_Keyboard = true;
		Controller_Joystick = 0;
	} else if( Controller < 0 ) {
		Controller_Assistance_AutoFlip = true;
		Controller_Assistance_AutoFire = true;
		Controller_Assistance_AutoFly = true;
	} else {
		Controller_Keyboard = false;
		Controller_Joystick = Controller;
	}
}

void Plane::Event( FwEvent* e )
{
	if( State == STATE_STALLED || State == STATE_EXPLODED || State == STATE_EXPLODING )
	{
		return;
	}

	if( Controller_Keyboard )
	{
		if( e->Type == EVENT_KEY_DOWN )
		{
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_LEFT:
					RotateLeft = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					RotateRight = true;
					break;
				case ALLEGRO_KEY_Z:
					HasShot = false;
					SetState( STATE_SHOOT );
					break;
				case ALLEGRO_KEY_X:
					if( State != STATE_SHOOT )
					{
						SetState( STATE_FLIPPING );
					}
					break;
				case ALLEGRO_KEY_D:
					SetState( STATE_HIT );
					break;
			}
		}
		if( e->Type == EVENT_KEY_UP )
		{
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_LEFT:
					RotateLeft = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					RotateRight = false;
					break;
			}
		}
	}

	if( (e->Type == EVENT_JOYSTICK_AXIS || e->Type == EVENT_JOYSTICK_BUTTON_DOWN || e->Type == EVENT_JOYSTICK_BUTTON_UP) && e->Data.Joystick.id == Controller_Joystick )
	{
		switch( e->Type )
		{
			case EVENT_JOYSTICK_AXIS:
				break;
			case EVENT_JOYSTICK_BUTTON_DOWN:
				switch( e->Data.Joystick.button )
				{
					case 0:
					case 2:
						HasShot = false;
						SetState( STATE_SHOOT );
						break;
					case 1:
					case 3:
						if( State != STATE_SHOOT )
						{
							SetState( STATE_FLIPPING );
						}
						break;
				}
				break;
			case EVENT_JOYSTICK_BUTTON_UP:
				break;
		}
	}


	if( Controller_Assistance_AutoFire )
	{
	}

	if( Controller_Assistance_AutoFly )
	{
		ProcessFlyingAI();
	}

}

void Plane::Update()
{
	GameObject::Update();

	Velocity += sin(Angle * (M_PI / 180.0)) / (Angle >= 180.0 ? 20.0 : 15.0);

	// Flip right way up
	if( Controller_Assistance_AutoFlip && State == STATE_FLYING && (((Angle > 270.0 || Angle <= 90.0) && Flipped) || (Angle > 90.0 && Angle <= 270.0 && !Flipped)) )
	{
		SetState( STATE_FLIPPING );
	}

	// Trying to ascend upside down
	if( (Angle > 270.0 && Flipped) || (Angle > 180.0 && Angle <= 270.0 && !Flipped) )
	{
		Velocity -= 0.03;
	}

	// People hiding offscreen
	if( Position->Y < 0.0 )
	{
		Velocity -= 0.03;
	}

	if( Velocity <= 0.0 )
	{
		Velocity = 0.0;
		if( State != STATE_STALLED )
		{
			RotateLeft = false;
			RotateRight = false;
			SetState( STATE_STALLED );
		}
	}

	if( State == STATE_STALLED )
	{
		if( Angle >= 270.0 || Angle <= 88.0 )
		{
			Angle += 3.0;
		} else if( Angle > 92.0 ) {
			Angle -= 3.0;
		} else {
			//Velocity *= 1.3;
		}
		if( Velocity >= PLANE_VELOCITY_STALLEXIT )
		{
			SetState( STATE_FLYING );
		}
	}

	if( State == STATE_FLIPPING )
	{
		if( Animation_CurrentFrame == 6 )
		{
			Flipped = !Flipped;
			SetState( STATE_FLYING );
		}
	}

	if( State == STATE_SHOOT )
	{
		if( Animation_CurrentFrame == 0 && !HasShot )
		{
			Game->AddGameObject( new Bullet( this ) );
			HasShot = true;
		}
		if( Animation_CurrentFrame == 1 )
		{
			SetState( STATE_FLYING );
		}
	}
	
	if( State != STATE_STALLED && State != STATE_EXPLODED && State != STATE_EXPLODING && State != STATE_SHOOT )
	{
		if( RotateLeft )
		{
			Angle = Angle - 1.0;
		}
		if( RotateRight )
		{
			Angle = Angle + 1.0;
		}
	}

	if( Velocity >= PLANE_VELOCITY_MAX )
	{
		Velocity = PLANE_VELOCITY_MAX;
	}

	if( (Health <= PLANE_DAMAGE_SMOKE_LOTS && Animation_Delay % (Animation_TicksPerFrame / 2) == 0) || (Health <= PLANE_DAMAGE_SMOKE_SMALL && Animation_Delay == 0) )
	{
		Game->AddGameObject( new SmokeCloud( Position, 0.8, Angle + 180.0 ) );
	}

}

void Plane::Render()
{
	if( ForRemoval )
	{
		return;
	}

	ALLEGRO_BITMAP* tileset = Game->GetGameScaledImage();
	int tileY = (Team * 32) * Game->graphicsMultiplier;
	int tileX = 144 * Game->graphicsMultiplier;

	if( State == STATE_FLIPPING )
	{
		tileX += 36 * Game->graphicsMultiplier;
	}

	ALLEGRO_BITMAP* tmp = al_create_sub_bitmap( tileset, tileX, tileY, 36 * Game->graphicsMultiplier, 32 * Game->graphicsMultiplier );
	al_draw_rotated_bitmap( tmp, 18 * Game->graphicsMultiplier, 16 * Game->graphicsMultiplier, Position->X * Game->graphicsMultiplier, Position->Y * Game->graphicsMultiplier, Angle * (M_PI / 180), (Flipped ? ALLEGRO_FLIP_VERTICAL : 0) );
	al_destroy_bitmap( tmp );
}

void Plane::SetState( int NewState )
{
	if( NewState == STATE_HIT )
	{
		LastSmokeFrame = 0;
		Health--;
		if( Health <= 0 )
		{
			NewState = STATE_EXPLODING;
		} else {
			// Just handle the hit, but let everything else continue
			NewState = State;
		}
	}
	State = NewState;
	Animation_CurrentFrame = 0;
}

void Plane::ProcessFlyingAI()
{
}
