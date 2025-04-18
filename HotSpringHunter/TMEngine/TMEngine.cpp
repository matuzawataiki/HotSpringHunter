#include "TMEnginePreCompile.h"
#include "TMEngine.h"
using namespace nsK2Engine;

namespace nsTMEngine
{
	SceneLight* g_sceneLight = nullptr;
	CollisionObjectManager* g_collisionObjectManager = nullptr;


	void TMEngine::Init()
	{
		g_collisionObjectManager = &m_collisionObjectManager;
	}

}