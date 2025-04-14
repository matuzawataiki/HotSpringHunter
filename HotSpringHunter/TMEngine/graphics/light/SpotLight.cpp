#include "TMEnginePreCompile.h"
namespace nsTMEngine 
{
	SpotLight::SpotLight()
	{
		m_spotLig = g_sceneLight->NewSpotLight();
	}
	SpotLight::~SpotLight()
	{
	}
	void SpotLight::Init(const Vector3& pos, const Vector3& color, const float& range, const Vector3& direction, const float angle)
	{
		SetPosition(pos);
		SetColor(color);
		SetRange(range);
		SetDirection(direction);
		SetAngle(angle);
	}
}