
#include "survivalgameover.h"
#include "game.h"

SurvivalGameOverStage::SurvivalGameOverStage()
{
	StageTime = 0;
}

void SurvivalGameOverStage::Begin()
{
}

void SurvivalGameOverStage::Pause()
{
}

void SurvivalGameOverStage::Resume()
{
}

void SurvivalGameOverStage::Finish()
{
}

void SurvivalGameOverStage::Event( FwEvent *e )
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keycode == ALLEGRO_KEY_ENTER || e->Data.Keyboard.keycode == ALLEGRO_KEY_ESCAPE || StageTime >= SRVLHIGHSCORE_MINTIMEOUT )
		{
			delete Framework::SystemFramework->ProgramStages->Pop();
			delete Framework::SystemFramework->ProgramStages->Pop();
		}
	}
	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN && StageTime >= SRVLHIGHSCORE_MINTIMEOUT )
	{
		delete Framework::SystemFramework->ProgramStages->Pop();
		delete Framework::SystemFramework->ProgramStages->Pop();
	}
}

void SurvivalGameOverStage::Update()
{
	if( StageTime < SRVLHIGHSCORE_MINTIMEOUT )
	{
		StageTime++;
	}
	Framework::SystemFramework->ProgramStages->Previous( this )->Update();
}

void SurvivalGameOverStage::Render()
{
	GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
	g->Render();
	al_draw_filled_rectangle( 0, 0, Framework::SystemFramework->GetDisplayWidth(), Framework::SystemFramework->GetDisplayHeight(), al_map_rgba( 0, 0, 0, 128 ) );

	ALLEGRO_FONT* survivalFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 18, 0 );
	ALLEGRO_COLOR survivalBorder = al_map_rgb( 0, 0, 0 );
	ALLEGRO_COLOR survivalNormal = al_map_rgb( 255, 255, 255 );

	int Lsiz = al_get_font_line_height( survivalFont );

}
