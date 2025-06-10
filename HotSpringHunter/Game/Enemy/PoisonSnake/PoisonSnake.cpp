#include "stdafx.h"
#include "PoisonSnake.h"
#include "Player.h"
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

		m_position.z += 500.0f;
		m_characterController.Init(30.0f, 50.0f, m_position);

	}
	PoisonSnake::~PoisonSnake()
	{
	}

	bool PoisonSnake::Start()
	{
		m_enemyModel.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
			});
		return true;
	}

	void PoisonSnake::Update()
	{

		ActivateStart();
		m_stateMachine->Update();

		m_position = m_characterController.Execute(m_moveSpeed, 1.0f);

		m_enemyModel.SetPosition(m_position);
		m_enemyModel.SetRotation(m_rotation);
		m_enemyModel.Update();
	}

	void PoisonSnake::ActivateStart()
	{

	}

	void PoisonSnake::Render(RenderContext& rc)
	{
		m_enemyModel.Draw(rc);
	}

	void PoisonSnake::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		if (wcscmp(eventName, L"Attack") == 0)
		{
			m_isAttack = true;
		}
	}

	void PoisonSnake::AttackOff()
	{
		m_isAttack = false;
		m_attackTime = ATTACK_TIME;
	}

	void PoisonSnake::AttackState()
	{
		if (!m_isAttack) {
			m_attackTime -= g_gameTime->GetFrameDeltaTime();
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