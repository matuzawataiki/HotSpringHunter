#include "k2EngineLowPreCompile.h"
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
		
		ModelInitData modelData;
		modelData.m_tkmFilePath = filePath;
		modelData.m_fxFilePath = "Assets/shader/model.fx";
		m_model.Init(modelData);

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
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

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_sca);
	}

	void ModelRender::Update()
	{
		UpdateWorldMatrixInModes();
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}

}