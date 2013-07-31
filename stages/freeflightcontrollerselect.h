
#pragma once

#include "../framework/framework.h"

class FreeFlightControllerSelectStage : public Stage
{
	private:
		void AddPlayer( ALLEGRO_JOYSTICK* joystick );

	public:
    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();
};
