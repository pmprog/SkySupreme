
#pragma once

#include "../framework/graphicslib.h"
#include "../framework/event.h"

#ifndef Framework
class Framework;
#endif // Framework

class NetworkManager
{

	private:
    Framework* systemFramework;

	public:
		NetworkManager( Framework* FrameworkObject );
		~NetworkManager();

		void Update();

};
