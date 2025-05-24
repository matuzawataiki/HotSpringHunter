#pragma once
#include <unordered_map>

namespace Enemy
{
	/// <summary>
	/// ステートの基底クラス
	/// </summary>
	class IState
	{
	public:
		IState(){}
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
}
