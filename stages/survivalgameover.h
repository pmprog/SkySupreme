
#pragma once

#include "../framework/framework.h"
#include "../objects/plane.h"

#define SRVLHIGHSCORE_MINTIMEOUT			FRAMES_PER_SECOND * 5
#define SRVLHIGHSCORE_KEYTIMEIN				FRAMES_PER_SECOND * 1

class SurvivalGameOverStage : public Stage
{

	private:
		int StageTime;
		Plane* playersPlane;

		int ActiveHighScore;
		int ActiveHighScoreChar;
		int CharSelect;

		static std::string CharacterList;
		static std::string LastName;

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
