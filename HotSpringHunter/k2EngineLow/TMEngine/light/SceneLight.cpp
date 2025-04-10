#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"
namespace nsK2EngineLow 
{
	SceneLight::SceneLight()
	{
		//ディレクションライトの設定
		m_light.m_drectionLight.SetDirection(1.0f, -1.0f, 1.0f);
		m_light.m_drectionLight.SetColor(0.3f, 0.3f, 0.3f);
		//カメラの位置の登録
		m_light.SetCameraPos();
		//環境光の設定
		m_light.SetAmbientLight(1.0f, 1.0f, 1.0f);
	}

	SceneLight::~SceneLight()
	{
	}

	SPointLight* SceneLight::NewPointLight()
	{
		//ライトの数が上限に達していたら作らない
		if (m_light.m_numPointLig >= MAX_POINT_LIGHT) {
			return nullptr;
		}

		//昇順でライトを登録
		for (int i = 0; i <= MAX_POINT_LIGHT;i++) {
			if (m_light.m_pointLight[i].m_isUse == false) {
				m_light.m_pointLight[i].Use();	//ライトを使用中にする
				m_light.m_numPointLig++;
				return &m_light.m_pointLight[i];
			}
		}
	}

	SSpotLight* SceneLight::NewSpotLight()
	{
		//ライトの数が上限に達していたら作らない
		if (m_light.m_numSpotLig >= MAX_SPOT_LIGHT) {
			return nullptr;
		}

		//昇順でライトを登録
		for (int i = 0; i <= MAX_SPOT_LIGHT;i++) {
			if (m_light.m_spotLight[i].m_isUse == false) {
				m_light.m_spotLight[i].Use();	//ライトを使用中にする
				m_light.m_numSpotLig++;
				return &m_light.m_spotLight[i];
			}
		}
	}

	void SceneLight::Init()
	{
	}

}

