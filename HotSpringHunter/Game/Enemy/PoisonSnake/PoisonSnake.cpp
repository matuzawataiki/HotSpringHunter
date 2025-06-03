#include "stdafx.h"
#include "PoisonSnake.h"
namespace Enemy
{
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

	void PoisonSnake::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		if (wcscmp(eventName, L"attack") == 0)
		{
			m_isAttack = true;
		}
	}

}