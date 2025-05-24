#include "stdafx.h"
#include "Towel.h"
#include "Player.h"

Towel::Towel()
{

}

Towel::~Towel() 
{

}

bool Towel::Start() 
{
	LoadModel();

	m_player = FindGO<Player>("player");

	return true;
}

/// <summary>
/// assets読み込み。
/// </summary>
void Towel::LoadModel()
{
	
	//アニメーション読み込み。
	m_animClips[enAnimClip_WeakAttack].Load("Assets/animData/towel/weakAttack.tka");
	m_animClips[enAnimClip_WeakAttack].SetLoopFlag(false);
	/*m_animClips[enAnimClip_ChargeAttack].Load("Assets/animData/towel/chargeAttack.tka");
	m_animClips[enAnimClip_ChargeAttack].SetLoopFlag(false);
	m_animClips[enAnimClip_Charging].Load("Assets/animData/towel/charging.tka");
	m_animClips[enAnimClip_Charging].SetLoopFlag(false);*/
	//モデル読み込み。
	m_towelModel.Init("Assets/modelData/towel/towel.tkm", m_animClips, enAnimClip_Num, enModelUpAxisY);
}

void Towel::Update()
{
	FollowPlayer();
	StateManage();
	ChargeAttackScale();
	DisplayManage();
	
	m_towelModel.SetPosition(m_towelPos);
	m_towelModel.SetRotation(m_towelRot);
	m_towelModel.SetScale(m_towelSca);
	m_towelModel.Update();
}

/// <summary>
/// player追従。
/// </summary>
void Towel::FollowPlayer()
{
	//位置を向きをplayerと同じにする。
	m_towelPos = m_player->GetPlayerPos();
	m_towelPos.y += 100.0f;
	m_towelDir = m_player->GetPlayerDir();

	m_towelRot.SetRotationYFromDirectionXZ(m_towelDir);
}

/// <summary>
/// アニメーション再生。
/// </summary>
void Towel::StateManage()
{
	/*switch (m_player->m_animationState) {
	case enChargeAttack:
		m_towelModel.PlayAnimation(enAnimClip_ChargeAttack);
		break;
	case m_player->enCharging:
		m_towelModel.PlayAnimation(enAnimClip_Charging);
		break;
	case m_player->enWeakAttack:
		m_towelModel.PlayAnimation(enAnimClip_WeakAttack);
		break;
	default:
		break;
	}*/
}

void Towel::ChargeAttackScale()
{
	/*if (m_player->m_animationState == m_player->enChargeAttack) {

	}
	else {
		m_towelSca = Vector3::One;
	}*/
}

/// <summary>
/// 表示状態切り替え。
/// </summary>
void Towel::DisplayManage()
{
	//アニメーションが再生中ならモデルを表示させる。
	/*if (m_towelModel.IsPlayAnimation()) {
		m_displayFlag = true;
	}
	else {
		m_displayFlag = false;
	}*/
	m_displayFlag = true;
}

void Towel::Render(RenderContext& rc) 
{
	if (m_displayFlag == true) {
		m_towelModel.Draw(rc);
	}	
}