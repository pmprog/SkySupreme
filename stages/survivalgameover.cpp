
#include "survivalgameover.h"
#include "game.h"

SurvivalGameOverStage::SurvivalGameOverStage( Plane* Player )
{
	StageTime = 0;
	playersPlane = Player;

	ActiveHighScore = -1;
	ActiveHighScoreChar = 0;

	ConfigFile* c = Framework::SystemFramework->Settings;

	if( c->KeyExists( "HiScores" ) )
	{
		for( int i = 0; i < c->GetArraySize( "HiScores" ); i++ )
		{
			if( c->GetQuickIntegerValue( "HiScores", i ) < Player->Score )
			{
				ActiveHighScore = i;
				break;
			}
		}
		if( ActiveHighScore < 0 && c->GetArraySize( "HiScores" ) < 10 )
		{
			ActiveHighScore = c->GetArraySize( "HiScores" );
		}
		if( ActiveHighScore >= 0 )
		{
			for( int i = min( c->GetArraySize( "HiScores" ) - 1, 9 ); i >= ActiveHighScore; i-- )
			{
				c->SetIntegerValue( "HiScores", i + 1, c->GetQuickIntegerValue( "HiScores", i ) );
				c->SetStringValue( "HiScoreName", i + 1, c->GetQuickStringValue( "HiScoreName", i ) );
			}
			c->SetIntegerValue( "HiScores", ActiveHighScore, Player->Score );
			c->SetStringValue( "HiScoreName", ActiveHighScore, new std::string( "..." ) );
		}

	} else {
		c->SetIntegerValue( "HiScores", 0, Player->Score );
		c->SetStringValue( "HiScoreName", 0, new std::string( "..." ) );
		ActiveHighScore = 0;
	}

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
		if( ((ActiveHighScore < 0 || ActiveHighScoreChar > 2) && e->Data.Keyboard.keycode == ALLEGRO_KEY_ENTER) || e->Data.Keyboard.keycode == ALLEGRO_KEY_ESCAPE || StageTime >= SRVLHIGHSCORE_MINTIMEOUT )
		{
			delete Framework::SystemFramework->ProgramStages->Pop();
			delete Framework::SystemFramework->ProgramStages->Pop();
		}
		switch( e->Data.Keyboard.keycode )
		{
			case ALLEGRO_KEY_ENTER:
				break;
			case ALLEGRO_KEY_DOWN:
				break;
			case ALLEGRO_KEY_UP:
				break;
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

	int qtrX = Framework::SystemFramework->GetDisplayWidth() / 4;
	ALLEGRO_FONT* LargeFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 14, 0 );
	ALLEGRO_FONT* NormalFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 18, 0 );
	ALLEGRO_COLOR fntBorder = al_map_rgb( 0, 0, 0 );
	ALLEGRO_COLOR fntNormal = al_map_rgb( 255, 255, 255 );
	ALLEGRO_COLOR fntSelect = al_map_rgb( 255, 220, 0 );

	int Lsiz = al_get_font_line_height( LargeFont );
	int Nsiz = al_get_font_line_height( NormalFont );
	ConfigFile* c = Framework::SystemFramework->Settings;

	al_draw_text( LargeFont, fntNormal, (qtrX * 2) - (al_get_text_width( LargeFont, "Game Over" ) / 2), 4, 0, "Game Over" );


	int yTop = 8 + Lsiz;
	al_draw_text( NormalFont, fntNormal, qtrX, yTop, 0, "Rank" );
	al_draw_text( NormalFont, fntNormal, qtrX * 2, yTop, 0, "Score" );
	al_draw_text( NormalFont, fntNormal, qtrX * 3, yTop, 0, "Name" );
	yTop += Nsiz + 1;

	for( int i = 0; i < min( c->GetArraySize( "HiScores" ), 10 ); i++ )
	{
		al_draw_textf( NormalFont, ( i == ActiveHighScore ? fntSelect : fntNormal ), qtrX, yTop, 0, "%d.", i + 1 );
		al_draw_textf( NormalFont, ( i == ActiveHighScore ? fntSelect : fntNormal ), qtrX * 2, yTop, 0, "%d", c->GetQuickIntegerValue( "HiScores", i ) );
		al_draw_textf( NormalFont, ( i == ActiveHighScore ? fntSelect : fntNormal ), qtrX * 3, yTop, 0, "%s", c->GetQuickStringValue( "HiScoreName", i )->c_str() );
		yTop += Nsiz + 1;
	}
	
}
