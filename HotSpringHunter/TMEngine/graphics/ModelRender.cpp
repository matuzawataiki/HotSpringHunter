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
		EnModelUpAxis enModelUpAxiz) 
	{
		//ƒXƒPƒ‹ƒgƒ“‚Ì‰Šú‰»
		InitSkeleton(filePath);
		//ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‰Šú‰»
		InitAnimation(animationeClips, numAnimationClips, enModelUpAxiz);
		
		ModelInitData modelInitData;			//ƒ‚ƒfƒ‹‚Ìƒf[ƒ^
		//tkm‚Ìƒtƒ@ƒCƒ‹ƒpƒX‚ÌŽw’è
		modelInitData.m_tkmFilePath = filePath;
		//ƒVƒF[ƒ_[‚Ìƒtƒ@ƒCƒ‹ƒpƒX‚ÌŽw’è
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		
		SetupShaderEntryPointFunc(modelInitData);

		if (animationeClips != nullptr) {
			modelInitData.m_skeleton = &m_skeleton;
		}

		//ã‚·ãƒ£ãƒ‰ã‚¦ãƒžãƒƒãƒ—ãƒ†ã‚¯ã‚¹ãƒãƒ£
		modelInitData.m_expandShaderResoruceView[4] = &g_renderingEngine->GetShadowMap();

		//ã‚·ãƒ¼ãƒ³ãƒ©ã‚¤ãƒˆ
		modelInitData.m_expandConstantBuffer = g_sceneLight->GetLight();
		modelInitData.m_expandConstantBufferSize = sizeof(Light);

		m_model.Init(modelInitData);		
	}

	void ModelRender::InitModelOnShadowMap(RenderingEngine& renderingEngine, const char* tkmFinlePath, EnModelUpAxis modelUpAxis)
	{
		//ã‚·ãƒ£ãƒ‰ã‚¦ãƒžãƒƒãƒ—æç”»ç”¨ãƒ¢ãƒ‡ãƒ«
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFinlePath;
		modelInitData.m_modelUpAxis = modelUpAxis;

		SetupShaderEntryPointFunc(modelInitData);

		if (m_animationClips != nullptr) {
			//ƒXƒPƒ‹ƒgƒ“‚ðŽw’è‚·‚éB
			modelInitData.m_skeleton = &m_skeleton;
		}

		modelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		m_drawShadowMapCameraParamCB.Init(sizeof(Vector4), nullptr);
		modelInitData.m_expandConstantBuffer = &m_drawShadowMapCameraParamCB;
		modelInitData.m_expandConstantBufferSize = sizeof(Vector4);
		m_shadowModels.Init(modelInitData);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc)
	{
		Vector4 cameraParam;
		cameraParam.x = g_camera3D->GetNear();
		cameraParam.y = g_camera3D->GetFar();
		m_drawShadowMapCameraParamCB.CopyToVRAM(cameraParam);
		m_shadowModels.Draw(rc);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		//tkm‚Ì’†g‚ðƒRƒs[
		int pos = (int)skeletonFilePath.find(".tkm");
		//ƒXƒPƒ‹ƒgƒ“‚Ìî•ñ‚ð‘‚«ž‚Ý
		skeletonFilePath.replace(pos, 4, ".tks");
		//charŒ^‚É•ÏŠ·‚µ‚ÄInit
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
			// ƒAƒjƒ[ƒVƒ‡ƒ“‚ ‚èB
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

		//ƒAƒjƒ[ƒVƒ‡ƒ“‚ði‚ß‚éB
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->RegisterModel(&m_model);
		g_renderingEngine->AddRenderObject(this);
	}

}