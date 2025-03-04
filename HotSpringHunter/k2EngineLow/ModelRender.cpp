#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
namespace nsTMEngine {
	ModelRender::ModelRender() {}
	ModelRender::~ModelRender() {}

	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationeClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis enModelUpAxiz = enModelUpAxisZ) 
	{
		//スケルトンの初期化
		InitSkeleton(filePath);
		//アニメーションの初期化
		InitAnimation(animationeClips, numAnimationClips, enModelUpAxiz);
		
		ModelInitData modelData;
		modelData.m_fxFilePath = filePath;
		m_model->Init(modelData);

	}
	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
	}
	void ModelRender::InitAnimation(AnimationClip* animtionClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
	}
	void ModelRender::InitComputeAnimatoinVertexBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis)
	{
	}
	void ModelRender::Update()
	{
	}
	void ModelRender::Draw(RenderContext& rc)
	{
	}
}