
#include "multicontrollerselect.h"
#include "game.h"

void MultipleControllerSelectStage::Begin()
{
}

void MultipleControllerSelectStage::Pause()
{
}

void MultipleControllerSelectStage::Resume()
{
}

void MultipleControllerSelectStage::Finish()
{
}

void MultipleControllerSelectStage::Event(FwEvent *e)
{
	switch( e->Type )
	{
		case EVENT_KEY_DOWN:
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_ESCAPE:
					delete Framework::SystemFramework->ProgramStages->Pop();
					delete Framework::SystemFramework->ProgramStages->Pop();
					break;
#ifdef PANDORA
				case ALLEGRO_KEY_PGUP:
				case ALLEGRO_KEY_PGDN:
				case ALLEGRO_KEY_HOME:
				case ALLEGRO_KEY_END:
#endif
				case ALLEGRO_KEY_Z:
				case ALLEGRO_KEY_X:
					AddPlayer( 0 );

					break;
				case ALLEGRO_KEY_ENTER:
					delete Framework::SystemFramework->ProgramStages->Pop();
					break;
			}
			break;

		case EVENT_JOYSTICK_BUTTON_DOWN:
			AddPlayer( e->Data.Joystick.id );
			break;
	}
}

void MultipleControllerSelectStage::Update()
{
	Framework::SystemFramework->ProgramStages->Previous( this )->Update();
}

void MultipleControllerSelectStage::Render()
{
	GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
	g->Render();
	al_draw_filled_rectangle( 0, 0, Framework::SystemFramework->GetDisplayWidth(), Framework::SystemFramework->GetDisplayHeight(), al_map_rgba( 0, 0, 0, 128 ) );

	int boxW = Framework::SystemFramework->GetDisplayWidth() / 5;
	int boxH = Framework::SystemFramework->GetDisplayHeight() / 4;
	std::list<Plane*>* players = g->GetPlaneObjects();
	ALLEGRO_BITMAP* tileset = g->GetGameScaledImage();

	int i = 0;
	for( std::list<Plane*>::const_iterator p = players->begin(); p != players->end(); p++ )
	{
		al_draw_filled_rounded_rectangle( (boxW * (i % 5)) + 2, (boxH * (i / 5)) + 2, (boxW * ((i + 1) % 5)) - 2, (boxH * ((i + 5) / 5)) - 2, 6, 6, al_map_rgba( 255, 255, 255, 128 ) );

		Plane* ply = (Plane*)*p;
		int tileX = 144 * g->graphicsMultiplier;
		int tileY = (ply->Team * 32) * g->graphicsMultiplier;
		ALLEGRO_BITMAP* tmp = al_create_sub_bitmap( tileset, tileX, tileY, 36 * g->graphicsMultiplier, 32 * g->graphicsMultiplier );
		al_draw_scaled_bitmap( tmp, 0, 0, 36 * g->graphicsMultiplier, 32 * g->graphicsMultiplier, (boxW * (i % 5)) + 4, (boxH * (i / 5)) + 4, (boxW * ((i + 1) % 5)) - 4, (boxH * ((i + 5) / 5)) - 4, 0 );
		al_destroy_bitmap( tmp );
		i++;
	}
}

void MultipleControllerSelectStage::AddPlayer( ALLEGRO_JOYSTICK* joystick )
{
	bool FoundPlayer = false;
	Plane* ply;

	// Launch with Joystick player
	GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
	std::list<Plane*>* l = g->GetPlaneObjects();

	for( std::list<Plane*>::iterator p = l->begin(); p != l->end(); p++ )
	{
		if( ((Plane*)*p)->Controller_Joystick == joystick )
		{
			ply = ((Plane*)*p);
			FoundPlayer = true;
			break;
		}
	}

	if( !FoundPlayer )
	{
		ply = g->AddPlayer( joystick );
		ply->Team = l->size() % 8;
	} else {
		ply->Team = (ply->Team + 1) % 8;
	}

}
