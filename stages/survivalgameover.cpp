
#include "survivalgameover.h"
#include "game.h"

std::string SurvivalGameOverStage::LastName = "AAA";
std::string SurvivalGameOverStage::CharacterList = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .?!";

SurvivalGameOverStage::SurvivalGameOverStage( Plane* Player )
{
	StageTime = 0;
	playersPlane = Player;

	ActiveHighScore = -1;
	ActiveHighScoreChar = 3;
	CharSelect = 0;

	ConfigFile* c = Framework::SystemFramework->Settings;

	if( c->KeyExists( "HiScoreLastName" ) )
	{
		std::string* v = c->GetQuickStringValue( "HiScoreLastName" );
		LastName.clear();
		LastName.append( v->c_str() );
	}

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
			ActiveHighScoreChar = 0;
			for( int i = min( c->GetArraySize( "HiScores" ) - 1, 9 ); i >= ActiveHighScore; i-- )
			{
				c->SetIntegerValue( "HiScores", i + 1, c->GetQuickIntegerValue( "HiScores", i ) );
				c->SetStringValue( "HiScoreName", i + 1, c->GetQuickStringValue( "HiScoreName", i ) );
			}
			c->SetIntegerValue( "HiScores", ActiveHighScore, Player->Score );
			c->SetStringValue( "HiScoreName", ActiveHighScore, new std::string( LastName ) );
		}

	} else {
		c->SetIntegerValue( "HiScores", 0, Player->Score );
		c->SetStringValue( "HiScoreName", 0, new std::string( LastName ) );
		ActiveHighScore = 0;
		ActiveHighScoreChar = 0;
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
	ConfigFile* c = Framework::SystemFramework->Settings;
	std::string* v;

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( (ActiveHighScoreChar > 2 && StageTime >= SRVLHIGHSCORE_MINTIMEOUT) || e->Data.Keyboard.keycode == ALLEGRO_KEY_ESCAPE )
		{
			delete Framework::SystemFramework->ProgramStages->Pop();
			delete Framework::SystemFramework->ProgramStages->Pop();
			return;
		}
		if( ActiveHighScore >= 0 && ActiveHighScoreChar < 3 && StageTime >= SRVLHIGHSCORE_KEYTIMEIN )
		{
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_ENTER:
					ActiveHighScoreChar = 3;
					break;

				case ALLEGRO_KEY_DOWN:
					v = c->GetQuickStringValue( "HiScoreName", ActiveHighScore );
					v->replace( ActiveHighScoreChar, 1, CharacterList.substr( (CharacterList.find( v->at( ActiveHighScoreChar ) ) + 1) % CharacterList.length(), 1 ) );
					c->SetStringValue( "HiScoreName", ActiveHighScore, v );
					LastName.clear();
					LastName.append( v->c_str() );
					c->SetStringValue( "HiScoreLastName", new std::string( LastName ) );
					break;
				case ALLEGRO_KEY_UP:
					v = c->GetQuickStringValue( "HiScoreName", ActiveHighScore );
					v->replace( ActiveHighScoreChar, 1, CharacterList.substr( (CharacterList.find( v->at( ActiveHighScoreChar ) ) + (CharacterList.length() - 1)) % CharacterList.length(), 1 ) );
					c->SetStringValue( "HiScoreName", ActiveHighScore, v );
					LastName.clear();
					LastName.append( v->c_str() );
					c->SetStringValue( "HiScoreLastName", new std::string( LastName ) );
					break;

#ifdef PANDORA
				case ALLEGRO_KEY_PGDN:
				case ALLEGRO_KEY_HOME:
				case ALLEGRO_KEY_END:
				case ALLEGRO_KEY_PGUP:
#endif
				case ALLEGRO_KEY_X:
				case ALLEGRO_KEY_Z:
					ActiveHighScoreChar++;
					break;
			}
		}
	}

	if( e->Type == EVENT_JOYSTICK_AXIS && ActiveHighScore >= 0 && ActiveHighScoreChar < 3 && StageTime >= SRVLHIGHSCORE_KEYTIMEIN )
	{
		if( e->Data.Joystick.axis == 1 )
		{
			if( e->Data.Joystick.pos < 0.0 )
			{
				v = c->GetQuickStringValue( "HiScoreName", ActiveHighScore );
				v->replace( ActiveHighScoreChar, 1, CharacterList.substr( (CharacterList.find( v->at( ActiveHighScoreChar ) ) + (CharacterList.length() - 1)) % CharacterList.length(), 1 ) );
				c->SetStringValue( "HiScoreName", ActiveHighScore, v );
				LastName.clear();
				LastName.append( v->c_str() );
				c->SetStringValue( "HiScoreLastName", new std::string( LastName ) );
			}

			if( e->Data.Joystick.pos > 0.0 )
			{
				v = c->GetQuickStringValue( "HiScoreName", ActiveHighScore );
				v->replace( ActiveHighScoreChar, 1, CharacterList.substr( (CharacterList.find( v->at( ActiveHighScoreChar ) ) + 1) % CharacterList.length(), 1 ) );
				c->SetStringValue( "HiScoreName", ActiveHighScore, v );
				LastName.clear();
				LastName.append( v->c_str() );
				c->SetStringValue( "HiScoreLastName", new std::string( LastName ) );
			}
		}
	}

	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
		if( ActiveHighScoreChar > 2 && StageTime >= SRVLHIGHSCORE_MINTIMEOUT )
		{
			delete Framework::SystemFramework->ProgramStages->Pop();
			delete Framework::SystemFramework->ProgramStages->Pop();
			return;
		}
		if( ActiveHighScore >= 0 && ActiveHighScoreChar < 3 && StageTime >= SRVLHIGHSCORE_KEYTIMEIN )
		{
			ActiveHighScoreChar++;
		}
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


	int yTop = 12 + Lsiz;
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
