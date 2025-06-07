#pragma once
#include "Enemy/State/IState.h"
#include "Enemy/PoisonSnake/PoisonSnake.h"


namespace Character {
	class Player;
}

namespace Enemy {
	//////////////////////////////////////////////////////////////////////////////
	// ポイズンスネークのステートマシン
	//////////////////////////////////////////////////////////////////////////////
	class PoisonSnakeStateMachine : public StateMachineBase
	{
	public:
		PoisonSnakeStateMachine();
		virtual ~PoisonSnakeStateMachine();

		template<typename T>
		inline void RegisterState()
		{
			m_stateMap.emplace(T::ID(), new T(this));
		}

		virtual void Update() override;

		/// <summary>
		/// ステートマシンの初期設定。必ず最初に呼び出す
		/// </summary>
		/// <param name="owner">自分のインスタンス</param>
		/// <param name="target">敵のインスタンス</param>
		void Init(PoisonSnake* owner, Character::Player* target) {
			m_owner = owner;
			m_target = target;
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 位置の取得
		/// </summary>
		/// <returns>位置</returns>
		inline const Vector3& GetPosition() {
			return m_owner->GetPosition();
		}
		/// <summary>
		/// 拡大率の取得
		/// </summary>
		/// <returns>拡大率</returns>
		inline const Vector3& GetScale() {
			return m_owner->GetScale();
		}
		/// <summary>
		/// 移動ベクトルの取得
		/// </summary>
		/// <returns>移動ベクトル</returns>
		inline const Vector3& GetMoveSpeed() {
			return m_owner->GetMoveSpeed();
		}
		/// <summary>
		/// 向きの取得
		/// </summary>
		/// <returns>向き</returns>
		inline const Quaternion& GetRotation() {
			return m_owner->GetRotation();
		}
		/// <summary>
		/// ターゲットのインスタンスを取得
		/// </summary>
		/// <returns>ターゲット</returns>
		inline Character::Player* GetTarget() {
			return m_target;
		}
		/// <summary>
		/// モデルレンダーを取得
		/// </summary>
		/// <returns>モデルレンダー</returns>
		inline ModelRender* GetModelRender() {
			return m_owner->GetModelRender();
		}

		/// <summary>
		/// 攻撃していいかどうかを返す
		/// </summary>
		/// <returns>攻撃判定</returns>
		inline bool GetIsAttack() {
			return m_owner->GetIsAttack();
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// エネミーの位置の設定
		/// </summary>
		/// <param name="position"></param>
		inline void SetOwnerPosition(Vector3& position) {
			m_owner->SetPosition(position);
		}

		/// <summary>
		/// エネミーの拡大率の設定
		/// </summary>
		/// <param name="position"></param>
		inline void SetOwnerScale(Vector3& scale) {
			m_owner->SetScale(scale);
		}

		/// <summary>
		/// 向きの設定
		/// </summary>
		/// <param name="rotetion">向き</param>
		inline void SetOwnerRotetion(const Quaternion& rotetion) {
			m_owner->SetRotetion(rotetion);
		}

		/// <summary>
		/// 移動速度の設定
		/// </summary>
		/// <param name="moveSpeed">移動速度</param>
		inline void SetOwnerMoveSpeed(const Vector3& moveSpeed) {
			m_owner->SetMoveSpeed(moveSpeed);
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 攻撃できるかどうか
		/// </summary>
		/// <returns>攻撃可能ならture、不可能ならfalse</returns>
		inline bool IsAttack() {
			return m_owner->IsAttack();
		}

		/// <summary>
		/// 自分に攻撃がヒットしたかどうか
		/// </summary>
		/// <returns>ヒットしたならture、してないならfalse</returns>
		inline bool IsHit() {
			return m_owner->IsHit();
		}

		/// <summary>
		/// 死んでいるかどうか
		/// </summary>
		/// <returns>死んでいるならture、生きているならfalse</returns>
		inline bool IsDeath() {
			return m_owner->IsDeath();
		}

		//////////////////////////////////////////////////////////////////////////////


		/// <summary>
		/// 攻撃判定をオフにする
		/// </summary>
		inline void AttackOff() {
			m_owner->AttackOff();
		}

	private:
		PoisonSnake* m_owner = nullptr;
		Character::Player* m_target = nullptr;
	};

	//////////////////////////////////////////////////////////////////////////////
	// 待機ステート
	//////////////////////////////////////////////////////////////////////////////

	class PoisonSnakeIdleState : public IState
	{
		appState(PoisonSnakeIdleState);
	public:
		PoisonSnakeIdleState(PoisonSnakeStateMachine* owner) : IState(), m_owner(owner) {}
		virtual ~PoisonSnakeIdleState() {}

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

		virtual bool RequestState(uint32_t& request) override;
	private:
		PoisonSnakeStateMachine* m_owner;

		float m_moveDirection = 1.0f;		//移動方向
		float m_moveTime = 0.0f;			//移動時間
		float m_idleTime = 0.0f;			//待機時間

		bool m_isIdle = false;				//止まるかどうか
		bool m_isIdleAnim = true;			//待機アニメーションを再生するかどうか
		bool m_isMoveAnim = true;			//移動アニメーションを再生するかどうか

	};

	//////////////////////////////////////////////////////////////////////////////
	// 攻撃ステート
	//////////////////////////////////////////////////////////////////////////////

	class PoisonSnakeAtkState : public IState
	{
		appState(PoisonSnakeIdleState);
	public:
		PoisonSnakeAtkState(PoisonSnakeStateMachine* owner) : IState(), m_owner(owner) {}
		virtual ~PoisonSnakeAtkState() {}

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

		virtual bool RequestState(uint32_t& request) override;
	private:
		PoisonSnakeStateMachine* m_owner;

		bool m_isAttack = false;	//攻撃していいかどうか


	};

	//////////////////////////////////////////////////////////////////////////////
	// 追従ステート
	//////////////////////////////////////////////////////////////////////////////

	class PoisonSnakeTrackState : public IState
	{
		appState(PoisonSnakeTrackState);
	public:
		PoisonSnakeTrackState(PoisonSnakeStateMachine* owner) : IState(), m_owner(owner) {}
		virtual ~PoisonSnakeTrackState() {}

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

		virtual bool RequestState(uint32_t& request) override;
	private:
		PoisonSnakeStateMachine* m_owner;

		float m_isMove = 1.0f; //手前と奥どちらのポイントに移動するか

	};

	//////////////////////////////////////////////////////////////////////////////
	// ノックバックステート
	//////////////////////////////////////////////////////////////////////////////

	class PoisonSnakeKnockBackState : public IState
	{
		appState(PoisonSnakeKnockBackState);
	public:
		PoisonSnakeKnockBackState(PoisonSnakeStateMachine* owner) : IState(), m_owner(owner) {}
		virtual ~PoisonSnakeKnockBackState() {}

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

		virtual bool RequestState(uint32_t& request) override;
	private:
		PoisonSnakeStateMachine* m_owner;

		Vector3	m_moveSpeed = Vector3::Zero;	//移動速度

		float m_isMove = 1.0f;		//手前と奥どちらのポイントに移動するか
		float m_knockDecreased = 1.0f;		//減衰率
	};

	//////////////////////////////////////////////////////////////////////////////
	//デスステート
	//////////////////////////////////////////////////////////////////////////////

	class PoisonSnakeDeathState : public IState
	{
		appState(PoisonSnakeDeathState);
	public:
		PoisonSnakeDeathState(PoisonSnakeStateMachine* owner) : IState(), m_owner(owner) {}
		virtual ~PoisonSnakeDeathState() {}

		virtual void Enter() override;
		virtual void Update() override;
		virtual void Exit() override;

		virtual bool RequestState(uint32_t& request) override;
	private:
		PoisonSnakeStateMachine* m_owner;

		float m_isMove = 1.0f;	//手前と奥どちらのポイントに移動するか

	};
}
