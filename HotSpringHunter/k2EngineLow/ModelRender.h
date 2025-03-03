#pragma oncen
#include "TMEnginePreCompile.h"

namespace nsTMEngine {
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// ���f���̏������p�֐�
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void Init(
			const char* filePath,
			AnimationClip* animationeClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ
			);

		/// <summary>
		/// �A�j���[�V�����̏������p�ϐ�
		/// </summary>
		/// <param name="animtionClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(
			AnimationClip* animtionClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);
		/// <summary>
		/// �X�P���g���̏������p�֐�
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();
		/// <summary>
		/// �`�揈���B
		/// </summary>
		void Draw(RenderContext& rc);

	};
}
