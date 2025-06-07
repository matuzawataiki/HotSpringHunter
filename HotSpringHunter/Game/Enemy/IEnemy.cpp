#include "stdafx.h"
#include "IEnemy.h"
#include "Player.h"
namespace Enemy {
	bool IEnemy::Start()
	{
		return true;
	}

	void IEnemy::Update()
	{
		m_target = FindGO<Character::Player>("player");		//不正アクセス防止のためUpdateでまわす。
	}

	void IEnemy::Render(RenderContext& rc)
	{
	}
}

