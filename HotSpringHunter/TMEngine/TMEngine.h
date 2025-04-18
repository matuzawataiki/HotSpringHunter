#pragma once
using namespace nsK2Engine;
namespace nsTMEngine {
	class SceneLight;
	class TMEngine
	{
	public:
		void Init();

	private:
		CollisionObjectManager m_collisionObjectManager;
	};

	extern SceneLight* g_sceneLight;
	extern CollisionObjectManager* g_collisionObjectManager;
}