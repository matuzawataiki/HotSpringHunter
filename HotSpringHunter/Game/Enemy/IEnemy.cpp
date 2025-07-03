#include "stdafx.h"
#include "IEnemy.h"
#include "Player.h"
namespace Enemy {
	IEnemy::IEnemy()
	{
		nlohmann::json j;
		j = LoadScene("Assets/json/test.json");

		m_moveSpeed = Vector3::Zero;
		m_position	= Vector3::Zero;
		m_scale		= Vector3::Zero;
		m_rotation	= Quaternion::Identity;
	}

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

