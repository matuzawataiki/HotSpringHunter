#include "TMEnginePreCompile.h"
#include "HemisphereLight.h"

namespace nsTMEngine
{
	HemisphereLight::HemisphereLight()
	{
		//”¼‹…ƒ‰ƒCƒg‚ðŽæ“¾
		m_hemisphereLight = g_sceneLight->GetHemisphereLight();
	}
	HemisphereLight::~HemisphereLight()
	{
	}
}