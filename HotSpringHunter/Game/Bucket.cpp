#include "stdafx.h"
#include "Bucket.h"
#include "Player.h"

Bucket::Bucket()
{

}

Bucket::~Bucket()
{

}

bool Bucket::Start()
{
	LoadAssets();

	m_player = FindGO<Player>("player");

	return true;
}

void Bucket::LoadAssets()
{
	//アニメーション読み込み。
	/*m_animClips[enAnimClip_GuardStart].Load("Assets/animData/oke/guardStart");
	m_animClips[enAnimClip_GuardStart].SetLoopFlag(false);
	m_animClips[enAnimClip_GuardEnd].Load("Assets/animData/oke/guardEnd");
	m_animClips[enAnimClip_GuardEnd].SetLoopFlag(false);*/

	//モデル読み込み。
	m_bucketModel.Init("Assets/modelData/oke/oke.tkm");
}

void Bucket::Update()
{
	FollowPlayer();
	ManageAnimState();

	m_bucketModel.SetPosition(m_bucketPos);
	m_bucketModel.SetRotation(m_bucketRot);
	m_bucketModel.Update();
}

/// <summary>
/// player追従。
/// </summary>
void Bucket::FollowPlayer()
{
	//位置と向きをplayerと同じにする。
	m_bucketPos = m_player->GetPlayerPos();
	m_bucketDir = m_player->GetPlayerDir();

	m_bucketRot.SetRotationYFromDirectionXZ(m_bucketDir);
}

/// <summary>
/// アニメーション再生。
/// </summary>
void Bucket::ManageAnimState()
{
	switch (m_player->m_animationState) {
	case m_player->enGuardStart:
		m_bucketModel.PlayAnimation(enAnimClip_GuardStart);
		break;
	default:
		break;
	}
}

/// <summary>
/// 表示状態切り替え。
/// </summary>
void Bucket::DisplayManage()
{
	//アニメーションが再生中ならモデルを表示させる。
	if (m_bucketModel.IsPlayAnimation() == true) {
		m_displayFlag = true;
	}
	else {
		m_displayFlag = false;
	}
}

void Bucket::Render(RenderContext& rc)
{
	if (m_displayFlag == true) {
		m_bucketModel.Draw(rc);
	}	
}