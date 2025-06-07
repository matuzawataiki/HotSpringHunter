#include "stdafx.h"
#include "PoisonSnake.h"
#include "Player.h"
#include "Enemy/State/PoisonSnakeState.h"

namespace {
	static float HP = 100.0f;
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

	}

	void PoisonSnake::ActivateStart()
	{

	}

	void PoisonSnake::Render(RenderContext& rc)
	{

	}

	void PoisonSnake::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		if (wcscmp(eventName, L"attack") == 0)
		{
			m_isAttack = true;
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
		m_animationClip[enAnimClip_Death].Load("Assets/animData/poisonSnake/attackAnimation.tka");
		m_animationClip[enAnimClip_Death].SetLoopFlag(true);

		//モデル読み込み
		m_enemyModel.Init("Assets/modelData/snake/poisonSnake/poisonSnake.tkm", m_animationClip, enAnimClip_Num, enModelUpAxisY);
	}

	void PoisonSnake::InitStateMachine()
	{
		m_stateMachine->Init(this, m_target);
		m_stateMachine->RegisterState<PoisonSnakeIdleState>();
		m_stateMachine->RegisterState<PoisonSnakeAtkState>();
		m_stateMachine->RegisterState<PoisonSnakeDeathState>();
		m_stateMachine->RegisterState<PoisonSnakeKnockBackState>();
		m_stateMachine->RegisterState<PoisonSnakeTrackState>();
	}

}