#pragma once
#include "TMEnginePreCompile.h"

namespace nsTMEngine {
	/// <summary>
	/// �X�L�����f�������_�[
	/// </summary>
	class ModelRender : public Noncopyable
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
			EnModelUpAxis enModelUpAxiz = enModelUpAxisZ);
		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f) {
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// �`�揈���B
		/// </summary>
		void Draw(RenderContext& rc);

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="pos">�ʒu</param>
		/// <param name="rot">��]</param>
		/// <param name="sce">�傫��</param>
		void SetTRS(const Vector3& pos, const Quaternion& rot, const Vector3& sca) {
			m_pos = pos;
			m_rot = rot;
			m_sca = sca;
		}
		/// <summary>
		/// �ʒu�̐ݒ�
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
		}
		/// <summary>
		/// �ʒu�̐ݒ�
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		void SetPosition(const float& x, const float& y, const float& z)
		{
			m_pos = Vector3(x, y, z);
		}
		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot) 
		{
			m_rot = rot;
		}
		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="sca"></param>
		void SetScale(const Vector3& sca)
		{
			m_sca = sca;
		}
		void SetScale(const float& x, const float& y, const float& z)
		{
			m_sca = Vector3(x, y, z);
		}

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
		/// �e�탂�f���̃��[���h�s��̍X�V
		/// </summary>
		void UpdateWorldMatrixInModes() ;
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update() ;

	private:

		Vector3			m_pos = Vector3::Zero;			//�ʒu
		Vector3			m_sca = Vector3::One;			//�傫��
		Quaternion		m_rot = Quaternion::Identity;	//��]

		Model			m_model;					//���f��
		Skeleton		m_skeleton;					//�{�[��
		Animation		m_animation;
		AnimationClip*	m_animationClips = nullptr;	//�A�j���[�V�����N���b�v
		int				m_numAnimationClips = 0;	//�A�j���[�V�����N���b�v�̐�

	};
}
