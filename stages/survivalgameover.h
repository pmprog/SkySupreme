
#pragma once

#include "../framework/framework.h"
#include "../objects/plane.h"

#define SRVLHIGHSCORE_MINTIMEOUT			FRAMES_PER_SECOND * 5

class SurvivalGameOverStage : public Stage
{

	private:
		int StageTime;
		Plane* playersPlane;

		int ActiveHighScore;
		int ActiveHighScoreChar;

	public:
		SurvivalGameOverStage( Plane* Player );
    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();

};
