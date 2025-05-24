#include "TMEnginePreCompile.h"
#include "HemisphereLight.h"

namespace nsTMEngine
{
	HemisphereLight::HemisphereLight()
	{
		//半球ライトを取得
		m_hemisphereLight = g_sceneLight->GetHemisphereLight();
	}
	HemisphereLight::~HemisphereLight()
	{
	}
}