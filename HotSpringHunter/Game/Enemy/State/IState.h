#pragma once
#include "Enemy/State/Util/CRC32.h"
#include <unordered_map>

#include "Enemy/PoisonSnake/PoisonSnake.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

namespace Enemy
{
	/// <summary>
	/// ステートの基底クラス
	/// </summary>
	class IState
	{
	public:
		IState() {}
		virtual ~IState() {}

		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;

		virtual bool RequestState(uint32_t& request) = 0;
	};

	class StateMachineBase
	{
		using StateMap = std::unordered_map<uint32_t, IState*>;

	protected:
		StateMap m_stateMap;
		IState* m_currentState;

	public:
		StateMachineBase()
			:m_currentState(nullptr)
		{
			m_stateMap.clear();
		}

		virtual ~StateMachineBase()
		{
			for (auto it : m_stateMap) {
				delete it.second;
				it.second = nullptr;
			}
			m_stateMap.clear();
		}

		virtual void Update() = 0;

	public:
		template<typename T>
		inline void RegisterState()
		{
			m_stateMap.emplace(T::ID(), new T());
		}

		template<typename T>
		inline void InitializeState()
		{
			//m_currentState = FindState
		}

	protected:
		inline IState* FindState(uint32_t id)
		{
			const auto& it = m_stateMap.find(id);
			if (it == m_stateMap.end())
			{
				return nullptr;
			}
			return it->second;
		}
	};

	//////////////////////////////////////////////////////////////////////////////
	// ポイズンスネークのステートマシン
	//////////////////////////////////////////////////////////////////////////////
	class PoisonSnakeStateMachine : public StateMachineBase
	{
	public:
		PoisonSnakeStateMachine();
		virtual ~PoisonSnakeStateMachine();

		template<typename T>
		inline void RegisterState(PoisonSnakeStateMachine* owner)
		{
			m_stateMap.emplace(T::ID(), new T());
		}

		virtual void Update() override;

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

		bool m_isAttack = false;	//攻撃していいか


	};


}
