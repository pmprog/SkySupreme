
#pragma once

#include "../framework/framework.h"

#define MPHIGHSCORE_MINTIMEOUT			FRAMES_PER_SECOND * 5

class MultiplayerHighScoreStage : public Stage
{

	private:
		int StageTime;

	public:
		MultiplayerHighScoreStage();

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();
};
