#include "stdafx.h"
#include "PoisonSnake.h"
#include "Player.h"
#include "EffectHub.h"
#include "EnemyHPBar.h"
#include "EnemyManager.h"
#include "Enemy/State/PoisonSnakeState.h"

namespace {
	static const float HP = 100.0f;
	static const float ATTACK_TIME = 10.0f;

}


namespace Enemy
{
	PoisonSnake::PoisonSnake()
	{
		m_hp = HP;
		m_hpMax = HP;

		m_target = FindGO<Character::Player>("player");

		LoadAssets();
		InitStateMachine();
	}

	PoisonSnake::~PoisonSnake()
	{
		EnemyManager* enemyManager = FindGO<EnemyManager>("enemyManager");
		enemyManager->DeleteEnemy(this);
		delete m_stateMachine;
	}

	bool PoisonSnake::Start()
	{
		m_enemyHPBar = NewGO<EnemyHPBar>(0, "enemyHPBar");
		m_enemyHPBar->Init(m_hp, m_position, m_target->GetPlayerPos());
		m_characterController.Init(30.0f, 50.0f, m_position);

		return true;
	}

	void PoisonSnake::Update()
	{
		if (!m_isAttackCooldown){
			AttackState();
		}
		HitCalculation();
		m_stateMachine->Update();

		m_position = m_characterController.Execute(m_moveSpeed, 1.0f);

		m_enemyModel.SetPosition(m_position);
		m_enemyModel.SetRotation(m_rotation);
		m_enemyModel.Update();

		m_enemyHPBar->UpdateHpBar(m_hp, m_position, m_target->GetPlayerPos());
	}

	void PoisonSnake::ActivateStart()
	{
	}

	void PoisonSnake::Render(RenderContext& rc)
	{
		m_enemyModel.Draw(rc);
	}

	void PoisonSnake::AttackOff()
	{
		m_isAttackCooldown = false;
		m_attackTime = ATTACK_TIME;
	}

	void PoisonSnake::AttackState()
	{

		if (!m_isAttackCooldown && m_attackTime >= 0) {
			m_attackTime -= g_gameTime->GetFrameDeltaTime();
		}
		if (m_attackTime < 0) {
			m_isAttackCooldown = true;
		}

	}

	void PoisonSnake::HitCalculation()
	{
		//被弾した場合
		if (m_target->m_collision->IsHit(m_characterController)) {

			//HPを減らす
			m_hp -= m_target->GetAttackPower();

			//被弾エフェクト
			//被弾エフェクト
			EffectEmitter* m_effect = NewGO<EffectEmitter>(0);
			m_effect->Init(EnEffectVar::enEnemyHit);
			Vector3 effectPos = m_position;
			effectPos.y += 30.0f;
			m_effect->SetPosition(effectPos);
			m_effect->SetRotation(Quaternion::Identity);
			m_effect->SetScale({ 10.0f,10.0f,10.0f });
			m_effect->Play();
			

			//HPがまだ残っている
			if (m_hp > 0.0f) {
				//ノックバック
				ChangeHitFlag();
			}
			//HPがなくなった
			else {
				//死亡（吹っ飛び）
				m_isDeath = true;
			}
			return;
		}
	}

	void PoisonSnake::LoadAssets()
	{
		//アニメーション読み込み
		m_animationClip[enAnimClip_Idle].Load("Assets/animData/snake/idle.tka");
		m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
		m_animationClip[enAnimClip_Run].Load("Assets/animData/snake/walk.tka");
		m_animationClip[enAnimClip_Run].SetLoopFlag(true);
		m_animationClip[enAnimClip_Attack].Load("Assets/animData/snake/attack.tka");
		m_animationClip[enAnimClip_Attack].SetLoopFlag(false);
		m_animationClip[enAnimClip_Hit].Load("Assets/animData/snake/hit.tka");
		m_animationClip[enAnimClip_Hit].SetLoopFlag(false);
		m_animationClip[enAnimClip_Death].Load("Assets/animData/snake/poisonSnake/attackAnimation.tka");
		m_animationClip[enAnimClip_Death].SetLoopFlag(true);

		//モデル読み込み
		m_enemyModel.Init("Assets/modelData/snake/poisonSnake/poisonSnake.tkm", m_animationClip, enAnimClip_Num, enModelUpAxisZ);
	}

	void PoisonSnake::InitStateMachine()
	{
		m_stateMachine = new PoisonSnakeStateMachine;

		m_stateMachine->Init(this, m_target);
		m_stateMachine->RegisterState<PoisonSnakeIdleState>();
		m_stateMachine->RegisterState<PoisonSnakeAtkState>();
		m_stateMachine->RegisterState<PoisonSnakeDeathState>();
		m_stateMachine->RegisterState<PoisonSnakeKnockBackState>();
		m_stateMachine->RegisterState<PoisonSnakeTrackState>();
		m_stateMachine->InitializeState<PoisonSnakeIdleState>();
	}



}