
#pragma once

#include "../framework/framework.h"

class MultipleControllerSelectStage : public Stage
{
	private:

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
