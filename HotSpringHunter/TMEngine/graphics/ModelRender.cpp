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
		//スケルトンの初期化
		InitSkeleton(filePath);
		//アニメーションの初期化
		InitAnimation(animationeClips, numAnimationClips, enModelUpAxiz);
		
		ModelInitData modelData;			//モデルのデータ
		//tkmのファイルパスの指定
		modelData.m_tkmFilePath = filePath;
		//シェーダーのファイルパスの指定
		modelData.m_fxFilePath = "Assets/shader/model.fx";

		//SetupVertexShaderEntryPointFunc(modelData);

		modelData.m_expandConstantBuffer = g_sceneLight->GetLight();
		modelData.m_expandConstantBufferSize = sizeof(Light);

		m_model.Init(modelData);

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;		//ストリング型の変数
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

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//modelInitData.m_vsSkinEntryPointFunc = "VSMainUsePreComputedVertexBuffer";
		//modelInitData.m_vsEntryPointFunc = "VSMainUsePreComputedVertexBuffer";

		//if (m_animationClips != nullptr) {
		//	// アニメーションあり。
		//	modelInitData.m_vsSkinEntryPointFunc = "VSMainSkinUsePreComputedVertexBuffer";
		//}
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
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
	}

}