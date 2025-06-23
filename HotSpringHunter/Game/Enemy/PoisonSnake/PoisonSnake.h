#pragma once
#include "Enemy/IEnemy.h"

namespace Character
{
	class Player;
}

class EnemyHPBar;

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

		/// <summary>
		/// 攻撃判定をオフにする
		/// </summary>
		void AttackOff();

	private:
		/// <summary>
		/// 攻撃の状態の管理
		/// </summary>
		void AttackState();

		/// <summary>
		/// アセットの読み込み
		/// </summary>
		void LoadAssets();

		/// <summary>
		/// ステートマシンの初期設定
		/// </summary>
		void InitStateMachine();

		/// <summary>
		/// ヒット計算
		/// </summary>
		void HitCalculation();

	private:
		PoisonSnakeStateMachine* m_stateMachine = nullptr;
		Character::Player* m_target = nullptr;
		EnemyHPBar* m_enemyHPBar = nullptr;

		AnimationClip m_animationClip[enAnimClip_Num];

	};

}
