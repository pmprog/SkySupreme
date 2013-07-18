
#include "smokecloud.h"
#include "../stages/game.h"

SmokeCloud::SmokeCloud( Vector2* StartPosition, double StartVelocity, double StartAngle ) : Particle( FRAMES_PER_SECOND, StartPosition, StartVelocity, StartAngle )
{
	Animation_TicksPerFrame = FRAMES_PER_SECOND / 9;
}

void SmokeCloud::Update()
{
	Particle::Update();
	if( Angle <= 90.0 || Angle > 274.0 )
	{
		Angle = Angle - 3.0;
	} else if( Angle > 90.0 && Angle < 266.0 ) {
		Angle = Angle + 3.0;
	}
}

void SmokeCloud::Render()
{
	if( ForRemoval )
	{
		return;
	}

	ALLEGRO_BITMAP* tileset = Game->GetGameScaledImage();
	int tileX = 0;
	int tileY = 64 * Game->graphicsMultiplier;

	switch( Animation_CurrentFrame )
	{
		case 0:
			tileX = 0;
			break;
		case 1:
		case 2:
			tileX = 16 * Game->graphicsMultiplier;
			break;
		case 3:
		case 4:
		case 5:
			tileX = 32 * Game->graphicsMultiplier;
			break;
		case 6:
		case 7:
			tileX = 48 * Game->graphicsMultiplier;
			break;
		default:
			ForRemoval = true;
			break;
	}
	al_draw_bitmap_region( tileset, tileX, tileY, 16 * Game->graphicsMultiplier, 16 * Game->graphicsMultiplier, (Position->X - 8) * Game->graphicsMultiplier, (Position->Y - 8) * Game->graphicsMultiplier, 0 );
	
}
