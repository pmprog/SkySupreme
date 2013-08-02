
#include "../framework/framework.h"
#include "../objects/object.h"
#include "../objects/bullet.h"
#include "../objects/cloud.h"
#include "../objects/explosion.h"
#include "../objects/plane.h"
#include "../objects/smokecloud.h"
#include <vector>
#include <list>

#define GAMEMODE_FREEFLIGHT						0
#define GAMEMODE_SURVIVAL							1
#define GAMEMODE_LASTMANSTANDING			2
#define GAMEMODE_TEAMBATTLES					3

#define SURVIVAL_INTERVALS						40

class GameStage : public Stage
{
  private:
		std::vector<GameObject*> Objects;
		std::list<int> ObjectsToRemove;
		std::list<GameObject*> ObjectsToAdd;

		int SurvivalDelay;
		int SurvivalTimer;
		int SurvivalTotalTimer;
		int SurvivalIndex;
		static int SurvivalArrivals[SURVIVAL_INTERVALS];

		void UpdateByRules();
		void UpdateSurvival();

		void RenderSurvival();

	public:

		int graphicsMultiplier;

		int Rules_GameMode;
		bool Rules_GameStarted;
		bool Rules_PlaneToPlaneCollisions;
		bool Rules_BulletToBulletCollisions;
		bool Rules_FriendlyFire;
		bool Rules_HasGround;

		GameStage( int Mode );

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();

    virtual void Event(FwEvent *e);
    virtual void Update();
    virtual void Render();

		ALLEGRO_BITMAP* GetGameScaledImage();
		void AddGameObject( GameObject* NewObject );
		Plane* AddPlayer( ALLEGRO_JOYSTICK* Controller );

		std::list<Plane*>* GetPlaneObjects();
		std::list<Bullet*>* GetBulletObjects();

		static ALLEGRO_BITMAP* GetGameImageAtScale( int Scale );
};
