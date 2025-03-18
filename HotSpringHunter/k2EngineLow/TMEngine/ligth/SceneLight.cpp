#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"
namespace nsK2EngineLow 
{
	SceneLight::SceneLight()
	{
		m_light.m_sDrectionLight.SetDirection(1.0f, -1.0f, 1.0f);
		m_light.m_sDrectionLight.SetColor(0.5f, 0.5f, 0.5f);
		m_light.m_sDrectionLight.SetCameraPos();
	}

	SceneLight::~SceneLight()
	{
	}

	void SceneLight::Init()
	{
	}

}

