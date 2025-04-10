#include "k2EngineLowPreCompile.h"
#include "HemisphereLight.h"

namespace nsK2EngineLow
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