#include "stdafx.h"
#include "EnemyHPBar.h"

namespace {
	const float BASE_OFFSET = 200.0f;
}

void EnemyHPBar::Init(const float hp, const Vector3& enemyPos, const Vector3& playerPos){
	m_maxHP = hp;
	m_HP = hp;
	m_enemyPos = enemyPos;
	m_playerPos = playerPos;

	m_backHpSprite.Init("Assets/sprite/BackHP.DDS", 102.0f, 22.0f,AlphaBlendMode_Trans);
	m_hpBarSprite.Init("Assets/sprite/HPBar.DDS", 100.0f, 20.0f, AlphaBlendMode_Trans);

	m_hpBarSprite.SetPivot(Vector2(0.0f, 0.5f));

	m_isInit = true;
}

void EnemyHPBar::SetHpBar(const float hp, const Vector3& enemyPos, const Vector3& playerPos)
{
	m_HP = hp;
	m_enemyPos = enemyPos;
	m_playerPos = playerPos;

}

void EnemyHPBar::Update()
{
	//Initをしていない場合実行しない
	if (!m_isInit) {
		return;
	}
	//ワールド座標からスクリーン座標に変換
	g_camera3D->CalcScreenPositionFromWorldPosition(m_spritePosition, m_enemyPos);

	Vector3 cameraToPos = m_enemyPos - g_camera3D->GetPosition();
	float distance = cameraToPos.Length();
	float scale = 1000.0f / (distance + 500.0f);
	float yOffset = BASE_OFFSET * scale;
	float xOffset = scale + 0.55f;

	m_spritePosition.y += yOffset;
	m_spritePosition.x *= xOffset;

	//座標をセット
	m_backHpSprite.SetPosition(m_spritePosition);

	//基点が違うので調整
	Vector2 hpBarPos = m_spritePosition;
	hpBarPos.x -= 50.0f;
	m_hpBarSprite.SetPosition(hpBarPos);

	//HPの残量によってHPBarの長さを変える
	float hpRatio = m_HP / m_maxHP;
	m_sca.x = hpRatio;
	if (hpBarPos.x <= 0.0f) {
		hpBarPos.x = 0.0f;
	}
	m_hpBarSprite.SetScale(m_sca);

	Vector3 playerToEnemy = m_enemyPos - m_playerPos;
	float diff = playerToEnemy.Length();
	if (diff >= 1500.0f) {
		m_isIn = false;
	}
	else {
		m_isIn = true;
	}

	m_hpBarSprite.Update();
	m_backHpSprite.Update();

}

void EnemyHPBar::Render(RenderContext& rc)
{
	if(m_isIn)
	{
		m_backHpSprite.Draw(rc);
		m_hpBarSprite.Draw(rc);
	}
}
