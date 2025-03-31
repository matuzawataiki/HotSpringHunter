#include "k2EngineLowPreCompile.h"
#include "HemisphereLight.h"

namespace nsK2EngineLow
{
	HemisphereLight::HemisphereLight()
	{
		m_hemisphereLight = g_sceneLight->GetHemisphereLight();
	}
	HemisphereLight::~HemisphereLight()
	{
	}
}