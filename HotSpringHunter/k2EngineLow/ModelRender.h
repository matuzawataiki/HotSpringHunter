#pragma oncen
#include "TMEnginePreCompile.h"

namespace nsTMEngine {
	/// <summary>
	/// �X�L�����f�������_�[
	/// </summary>
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// ���f���̏������p�֐�
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationeClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����̐�</param>
		/// <param name="enModelUpAxiz">���f���̏����</param>
		void Init(
			const char* filePath,
			AnimationClip* animationeClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ
			);
	private:
		/// <summary>
		/// �X�P���g���̏������p�֐�
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);
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
		/// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z������������
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitComputeAnimatoinVertexBuffer(
			const char* tkmFilePath,
			EnModelUpAxis enModelUpAxis);
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();
		/// <summary>
		/// �`�揈���B
		/// </summary>
		void Draw(RenderContext& rc);

	private:
		Model* m_model = nullptr;	//���f��
		Skeleton m_sukeleton;		//�{�[��

	};
}
