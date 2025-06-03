#pragma once
#include "Enemy/EnemyBase.h"

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

	class PoisonSnake : public EnemyBase
	{
	private:
		bool Start();

	public:
		~PoisonSnake();
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
		inline void AttackOff() {
			m_isAttack = false;
		}

	private:
		bool m_isAttack = false;	//攻撃していいかどうか
	};

}
