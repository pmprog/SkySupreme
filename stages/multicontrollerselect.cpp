
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
					// Launch with Joystick player
					//GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
					//Plane* ply = new Plane( 0, new Vector2( 20, 20 ), PLANE_VELOCITY_MAX * 0.7, 0.0 );
					//g->AddGameObject( ply );
					break;
			}
			break;

		case EVENT_JOYSTICK_BUTTON_DOWN:
			// Launch with Joystick player
			//GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
			//Plane* ply = new Plane( e->Data.Joystick.id, new Vector2( 20, 20 ), PLANE_VELOCITY_MAX * 0.7, 0.0 );
			//g->AddGameObject( ply );
			break;
	}
}

void MultipleControllerSelectStage::Update()
{
}

void MultipleControllerSelectStage::Render()
{
	Framework::SystemFramework->ProgramStages->Previous( this )->Render();
	al_draw_filled_rectangle( 0, 0, Framework::SystemFramework->GetDisplayWidth(), Framework::SystemFramework->GetDisplayHeight(), al_map_rgba( 0, 0, 0, 128 ) );

	/*
	ALLEGRO_FONT* menuFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 12, 0 );
	int Lsiz = al_get_font_line_height( menuFont );
	int Ypos = (Framework::SystemFramework->GetDisplayHeight() / 2) - al_get_font_line_height( menuFont );
	int Xpos = (Framework::SystemFramework->GetDisplayWidth() / 2);
	int Xsiz;
	
	Xsiz = al_get_text_width( menuFont, "Press Fire" );
	al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), Xpos - (Xsiz / 2), Ypos, 0, "Press Fire" );
	Ypos += Lsiz;
	Xsiz = al_get_text_width( menuFont, "To Start" );
	al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), Xpos - (Xsiz / 2), Ypos, 0, "To Start" );
	*/
}
