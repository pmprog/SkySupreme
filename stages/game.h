
#include "../framework/framework.h"
#include "../objects/object.h"
#include <vector>
#include <list>

class GameStage : public Stage
{
  private:
		std::vector<GameObject*> Objects;
		std::list<int> ObjectsToRemove;
		

	public:
		int graphicsMultiplier;

		GameStage();

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();

    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();

		ALLEGRO_BITMAP* GetGameScaledImage();

};
