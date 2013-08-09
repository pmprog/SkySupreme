
#include "multiplayermode.h"
#include "game.h"

void MultiplayerModeStage::Begin()
{
	Selected = 0;
}

void MultiplayerModeStage::Pause()
{
}

void MultiplayerModeStage::Resume()
{
}

void MultiplayerModeStage::Finish()
{
}

void MultiplayerModeStage::Event(FwEvent *e)
{
	switch( e->Type )
	{
		case EVENT_KEY_DOWN:
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_ESCAPE:
					delete Framework::SystemFramework->ProgramStages->Pop();
					break;
				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_RIGHT:
					Selected = 1 - Selected;
					break;
#ifdef PANDORA
				case ALLEGRO_KEY_PGUP:
				case ALLEGRO_KEY_PGDN:
				case ALLEGRO_KEY_HOME:
				case ALLEGRO_KEY_END:
#endif
				case ALLEGRO_KEY_Z:
				case ALLEGRO_KEY_X:
				case ALLEGRO_KEY_ENTER:
					StartGameMode( GAMEMODE_LASTMANSTANDING + Selected );
					break;
			}
			break;

		case EVENT_JOYSTICK_BUTTON_DOWN:
			StartGameMode( GAMEMODE_LASTMANSTANDING + Selected );
			break;
	}
}

void MultiplayerModeStage::Update()
{
}

void MultiplayerModeStage::Render()
{
	ALLEGRO_FONT* optionFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 12, 0 );

	Framework::SystemFramework->ProgramStages->Previous( this )->Render();
	al_draw_filled_rectangle( 0, 0, Framework::SystemFramework->GetDisplayWidth(), Framework::SystemFramework->GetDisplayHeight(), al_map_rgba( 0, 0, 0, 128 ) );

	int boxW = Framework::SystemFramework->GetDisplayWidth() / 2.5;
	int boxH = Framework::SystemFramework->GetDisplayHeight() / 4;

	al_draw_filled_rounded_rectangle( (Framework::SystemFramework->GetDisplayWidth() / 2) - boxW - 3, (Framework::SystemFramework->GetDisplayHeight() / 2) - (boxH / 2), (Framework::SystemFramework->GetDisplayWidth() / 2) - 3, (Framework::SystemFramework->GetDisplayHeight() / 2) + (boxH / 2), 8, 8, (Selected == 0 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 0, 0, 0 )) );
	al_draw_filled_rounded_rectangle( (Framework::SystemFramework->GetDisplayWidth() / 2) + 3, (Framework::SystemFramework->GetDisplayHeight() / 2) - (boxH / 2), (Framework::SystemFramework->GetDisplayWidth() / 2) + boxW + 3, (Framework::SystemFramework->GetDisplayHeight() / 2) + (boxH / 2), 8, 8, (Selected == 1 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 0, 0, 0 )) );

	al_draw_text( optionFont, (Selected != 0 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 0, 0, 0 )), (Framework::SystemFramework->GetDisplayWidth() / 2) - boxW, (Framework::SystemFramework->GetDisplayHeight() / 2), 0, "Last Man Standing" );
	al_draw_text( optionFont, (Selected != 1 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 0, 0, 0 )), (Framework::SystemFramework->GetDisplayWidth() / 2) + 3, (Framework::SystemFramework->GetDisplayHeight() / 2), 0, "Team Battles" );

}

void MultiplayerModeStage::StartGameMode( int Mode )
{
	Framework::SystemFramework->ProgramStages->Pop();
	Framework::SystemFramework->ProgramStages->Push( (Stage*)new GameStage( Mode ) );
}
