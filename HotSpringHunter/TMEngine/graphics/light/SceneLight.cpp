#include "TMEnginePreCompile.h"
#include "SceneLight.h"
namespace nsTMEngine
{
	SceneLight::SceneLight()
	{
		
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
		//ディレクションライトの設定
		m_light.m_drectionLight.SetDirection(0.0f, -1.0f, 1.0f);
		m_light.m_drectionLight.SetColor(0.7f, 0.7f, 0.7f);
		//カメラの位置の登録
		m_light.m_cameraPos = g_camera3D->GetPosition();
		m_light.m_drectionLight.m_LVP = g_camera3D->GetViewProjectionMatrix();
		//環境光の設定
		m_light.SetAmbientLight(0.5f, 0.5f, 0.5f);
	}

	void SPointLight::Update()
	{
		m_posInView = m_pos;
		g_camera3D->GetViewMatrix().Apply(m_posInView);

	}

	void SSpotLight::Update()
	{
		m_posInView = m_pos;
		g_camera3D->GetViewMatrix().Apply(m_posInView);
	}

	void SceneLight::Update()
	{
		Matrix LVP;
		Vector3 upAxis = { 0.0f,1.0f,0.0f };
		// ライトをカメラと見立てたビュー行列を計算する
		Matrix viewMatrix;
		viewMatrix.MakeLookAt(g_camera3D->GetPosition(), g_camera3D->GetTarget(), upAxis);
		// プロジェクション行列を計算する
		Matrix projMatrix;
		float shadowNear = g_camera3D->GetNear() - 4000;
		float shadowFar = g_camera3D->GetFar() + 6000;
		projMatrix.MakeOrthoProjectionMatrix(6000, 3000, shadowNear, shadowFar);
		LVP = viewMatrix * projMatrix;
		m_light.m_drectionLight.UpdateLVP(LVP);
		
	}

}