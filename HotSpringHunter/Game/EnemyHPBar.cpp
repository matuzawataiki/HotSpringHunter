#include "stdafx.h"
#include "EnemyHPBar.h"

void EnemyHPBar::Init(const float hp, const Vector3& pos){
	m_maxHP = hp;
	m_HP = hp;
	m_pos = pos;

	m_backHpSprite.Init("Assets/sprite/BackHP.DDS", 255.0f, 55.0f,AlphaBlendMode_Trans);
	m_hpBarSprite.Init("Assets/sprite/HPBar.DDS", 250.0f, 50.0f, AlphaBlendMode_Trans);

	m_hpBarSprite.SetPivot(Vector2(0.0f, 0.5f));

	m_isInit = true;
}

void EnemyHPBar::SetHpBar(const float hp, const Vector3& pos)
{
	m_HP = hp;
	m_pos = pos;
}

void EnemyHPBar::Update()
{
	//Initをしていない場合実行しない
	if (!m_isInit) {
		return;
	}
	Vector3 pos = m_pos;
	//少し上にする
	pos.y += 200.0f;
	//ワールド座標からスクリーン座標に変換
	g_camera3D->CalcScreenPositionFromWorldPosition(m_spritePosition, pos);

	m_backHpSprite.SetPosition(m_spritePosition);

	Vector2 hpBarPos = m_spritePosition;
	hpBarPos.x -= 125.0f;
	m_hpBarSprite.SetPosition(hpBarPos);

	float hpRatio = m_HP / m_maxHP;
	m_sca.x = hpRatio;
	if (hpBarPos.x <= 0.0f) {
		hpBarPos.x = 0.0f;
	}
	m_hpBarSprite.SetScale(m_sca);

	m_hpBarSprite.Update();
	m_backHpSprite.Update();

}

void EnemyHPBar::Render(RenderContext& rc)
{
	m_backHpSprite.Draw(rc);
	m_hpBarSprite.Draw(rc);
}
