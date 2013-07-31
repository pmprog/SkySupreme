
#include "smokecloud.h"
#include "../stages/game.h"

Explosion::Explosion( Vector2* StartPosition ) : Particle( FRAMES_PER_SECOND / 2, StartPosition, 0.0, 0.0 )
{
	Animation_TicksPerFrame = FRAMES_PER_SECOND / 18;
}

void Explosion::Update()
{
	Particle::Update();
}

void Explosion::Render()
{
	if( ForRemoval )
	{
		return;
	}

	ALLEGRO_BITMAP* tileset = Game->GetGameScaledImage();
	int tileX = (Animation_CurrentFrame % 4) * 32 * Game->graphicsMultiplier;
	int tileY = (96 + ((Animation_CurrentFrame / 4) * 32)) * Game->graphicsMultiplier;

	al_draw_bitmap_region( tileset, tileX, tileY, 32 * Game->graphicsMultiplier, 32 * Game->graphicsMultiplier, (Position->X - 16) * Game->graphicsMultiplier, (Position->Y - 16) * Game->graphicsMultiplier, 0 );
}
