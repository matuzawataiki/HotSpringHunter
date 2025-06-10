#pragma once
#include "Enemy/IEnemy.h"

namespace Character
{
	class Player;
}

namespace Enemy
{
	enum EnEnemyAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Run,
		enAnimClip_Attack,
		enAnimClip_Hit,
		enAnimClip_Death,
		enAnimClip_Num,
	};

	class PoisonSnakeStateMachine;
	class PoisonSnake : public IEnemy
	{
	public:
		PoisonSnake();
		virtual ~PoisonSnake();

		bool Start() override;
		void Update() override;
		void ActivateStart() override;
		void Render(RenderContext& rc) override;

		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

		/// <summary>
		/// 攻撃していいかどうかを返す
		/// </summary>
		/// <returns>攻撃判定</returns>
		inline bool GetIsAttack() {
			return m_isAttack;
		}

		/// <summary>
		/// 攻撃判定をオフにする
		/// </summary>
		void AttackOff();

	private:
		/// <summary>
		/// 攻撃の状態の管理
		/// </summary>
		void AttackState();

		void LoadAssets();

		void InitStateMachine();

	private:
		PoisonSnakeStateMachine* m_stateMachine = nullptr;
		Character::Player* m_target = nullptr;

		AnimationClip m_animationClip[enAnimClip_Num];

	};

}
