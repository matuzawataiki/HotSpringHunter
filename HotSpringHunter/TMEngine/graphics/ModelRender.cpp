#include "TMEnginePreCompile.h"
#include "ModelRender.h"

namespace nsTMEngine {

	ModelRender::ModelRender() {
	}

	ModelRender::~ModelRender() {
	}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationeClips,
		int numAnimationClips,
		bool islighting,
		EnModelUpAxis enModelUpAxiz) 
	{
		//スケルトンの初期化
		InitSkeleton(filePath);
		//アニメーションの初期化
		InitAnimation(animationeClips, numAnimationClips, enModelUpAxiz);
		
		ModelInitData modelInitData;			//モデルのデータ
		//tkmのファイルパスの指定
		modelInitData.m_tkmFilePath = filePath;
		//シェーダーのファイルパスの指定
		if(islighting)
		{
			modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		}
		else {
			modelInitData.m_fxFilePath = "Assets/shader/lightOffModel.fx";
		}
		
		SetupShaderEntryPointFunc(modelInitData);

		if (animationeClips != nullptr) {
			modelInitData.m_skeleton = &m_skeleton;
		}

		//シャドウマップテクスチャ
		modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine->GetShadowMap();

		//シーンライト
		modelInitData.m_expandConstantBuffer = g_sceneLight->GetLight();
		modelInitData.m_expandConstantBufferSize = sizeof(Light);
		m_model.Init(modelInitData);

		//シャドウマップテクスチャ
		modelInitData.m_expandShaderResoruceView[0] = nullptr;

		modelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		//m_drawShadowMapCameraParamCB.Init(sizeof(Vector4), nullptr);
		//modelInitData.m_expandConstantBuffer = &m_drawShadowMapCameraParamCB;
		//modelInitData.m_expandConstantBufferSize = sizeof(Vector4);
		modelInitData.m_expandConstantBuffer = &g_sceneLight->GetLVP();
		m_shadowModels.Init(modelInitData);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc)
	{
		//Vector4 cameraParam;
		//cameraParam.x = g_camera3D->GetNear();
		//cameraParam.y = g_camera3D->GetFar();
		//m_drawShadowMapCameraParamCB.CopyToVRAM(cameraParam);
		m_shadowModels.Draw(rc);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		//tkmの中身をコピー
		int pos = (int)skeletonFilePath.find(".tkm");
		//スケルトンの情報を書き込み
		skeletonFilePath.replace(pos, 4, ".tks");
		//char型に変換してInit
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animtionClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animtionClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init(
				m_skeleton,
				m_animationClips,
				numAnimationClips);
		}

	}

	void ModelRender::SetupShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		modelInitData.m_vsEntryPointFunc = "VSMain";
		modelInitData.m_vsSkinEntryPointFunc = "VSMain";


		if (m_animationClips != nullptr) {
			// アニメーションあり。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
		m_shadowModels.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModes();

		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//アニメーションを進める。
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->RegisterModel(&m_model);
		g_renderingEngine->AddRenderObject(this);
	}

}