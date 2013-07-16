
#include "../framework/framework.h"

class MenuStage : public Stage
{
  private:
		int OptionSelected;
		ALLEGRO_COLOR OptionGlow[6];
		int OptionGlowIndex;
		int OptionGlowDelay;

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
