
#include "multiplayerhighscore.h"
#include "game.h"

void MultiplayerHighScoreStage::Begin()
{
}

void MultiplayerHighScoreStage::Pause()
{
}

void MultiplayerHighScoreStage::Resume()
{
}

void MultiplayerHighScoreStage::Finish()
{
}

void MultiplayerHighScoreStage::Event(FwEvent *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keycode == ALLEGRO_KEY_ENTER || e->Data.Keyboard.keycode == ALLEGRO_KEY_ESCAPE )
		{
			delete Framework::SystemFramework->ProgramStages->Pop();
			delete Framework::SystemFramework->ProgramStages->Pop();
		}
	}
}

void MultiplayerHighScoreStage::Update()
{
	Framework::SystemFramework->ProgramStages->Previous( this )->Update();
}

void MultiplayerHighScoreStage::Render()
{
	int Xpos;

	GameStage* g = (GameStage*)Framework::SystemFramework->ProgramStages->Previous( this );
	g->Render();
	al_draw_filled_rectangle( 0, 0, Framework::SystemFramework->GetDisplayWidth(), Framework::SystemFramework->GetDisplayHeight(), al_map_rgba( 0, 0, 0, 128 ) );

	ALLEGRO_FONT* survivalFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 18, 0 );
	ALLEGRO_COLOR survivalBorder = al_map_rgb( 0, 0, 0 );
	ALLEGRO_COLOR survivalNormal = al_map_rgb( 255, 255, 255 );

	int Lsiz = al_get_font_line_height( survivalFont );

	int boxW = Framework::SystemFramework->GetDisplayWidth() / 5;
	int boxH = Framework::SystemFramework->GetDisplayHeight() / 4;
	std::list<Plane*>* players = g->GetAllPlaneObjects();
	ALLEGRO_BITMAP* tileset = g->GetGameScaledImage();

	int i = 0;
	for( std::list<Plane*>::const_iterator p = players->begin(); p != players->end(); p++ )
	{
		al_draw_filled_rounded_rectangle( (boxW * (i % 5)) + 2, (boxH * (i / 5)) + 2, (boxW * (i % 5)) + boxW - 2, (boxH * ((i + 5) / 5)) - 2, 6, 6, al_map_rgba( 255, 255, 255, 128 ) );

		Plane* ply = (Plane*)*p;
		int tileX = 144 * g->graphicsMultiplier;
		int tileY = (ply->Team * 48) * g->graphicsMultiplier;
		ALLEGRO_BITMAP* tmp = al_create_sub_bitmap( tileset, tileX, tileY, 48 * g->graphicsMultiplier, 48 * g->graphicsMultiplier );
		al_draw_scaled_bitmap( tmp, 0, 0, 48 * g->graphicsMultiplier, 48 * g->graphicsMultiplier, (boxW * (i % 5)) + 4, (boxH * (i / 5)) + 4, boxW - 4, boxH - 4, 0 );
		al_destroy_bitmap( tmp );

		Xpos = (boxW / 2) - (al_get_text_width( survivalFont, "Score" ) / 2);
		al_draw_text( survivalFont, survivalNormal, (boxW * (i % 5)) + Xpos, (boxH * (i / 5)) + 4, 0, "Score" );

		char scoreArea[20];
		memset( (void*)&scoreArea, 0, 20 );
		sprintf( (char*)&scoreArea, "%d", ply->Score );

		Xpos = (boxW / 2) - (al_get_text_width( survivalFont, (char*)&scoreArea ) / 2);
		al_draw_text( survivalFont, survivalNormal, (boxW * (i % 5)) + Xpos, (boxH * (i / 5)) + boxH - 4 - Lsiz, 0, (char*)&scoreArea );

		i++;
	}



	Xpos = (Framework::SystemFramework->GetDisplayWidth() / 2) - (al_get_text_width( survivalFont, "Press Enter To Return To Menu" ) / 2);

	for( int yBrd = -1; yBrd < 2; yBrd++ )
	{
		for( int xBrd = -1; xBrd < 2; xBrd++ )
		{
			al_draw_text( survivalFont, survivalBorder, Xpos + xBrd, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 6 + yBrd, 0, "Press Enter To Return To Menu" );
		}
	}

	al_draw_text( survivalFont, survivalNormal, Xpos, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 6, 0, "Press Enter To Return To Menu" );

}
