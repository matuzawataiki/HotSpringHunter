#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"
namespace nsK2EngineLow {
	DirectionLight::DirectionLight()
	{
		m_directionLig.SetDirection(1.0f, -1.0f, -1.0f);
		m_directionLig.SetColor(0.5, 0.5, 0.5);
	}

	DirectionLight::~DirectionLight()
	{
	}
}
