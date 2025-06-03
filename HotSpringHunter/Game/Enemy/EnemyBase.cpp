#include "stdafx.h"
#include "EnemyBase.h"
#include "Player.h"
namespace Enemy {
	bool EnemyBase::Start()
	{

	}

	void EnemyBase::Update()
	{
		m_target = FindGO<Character::Player>("player");		//不正アクセス防止のためUpdateでまわす。

	}

	void EnemyBase::Render(RenderContext& rc)
	{

	}
}

