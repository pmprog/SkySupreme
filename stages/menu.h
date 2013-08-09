
#pragma once

#include "../framework/framework.h"
#include "../objects/cloud.h"

#define MENU_CLOUD_COUNT	20

class MenuStage : public Stage
{
  private:
		int OptionSelected;
		ALLEGRO_COLOR OptionGlow[6];
		int OptionGlowIndex;
		int OptionGlowDelay;
		std::list<Cloud*> BackgroundClouds;
		ALLEGRO_BITMAP* tileSet;
		int tileMultiplier;

		bool MusicOn;

		bool WasJoyUp;
		bool WasJoyDown;

		void ProcessMenuOption();

	public:
		MenuStage();

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();

    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();

};
