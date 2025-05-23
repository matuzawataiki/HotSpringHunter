#pragma once
class EnemyHPBar: public IGameObject
{
public:
	/// <summary>
	/// HPの初期設定
	/// </summary>
	/// <param name="hp">体力</param>
	/// <param name="pos">位置</param>
	void Init(const float hp, const Vector3& enemyPos, const Vector3& playerPos);

	void SetHpBar(const float hp, const Vector3& enemyPos, const Vector3& playerPos);

private:
	void Update();

	void Render(RenderContext& rc);

private:
	SpriteRender m_backHpSprite;		//HPの背景画像
	SpriteRender m_hpBarSprite;			//HPバーの画像

	Vector3		m_enemyPos = Vector3::Zero;				//敵モデルの位置
	Vector3		m_playerPos = Vector3::Zero;			//プレイヤーモデルの位置
	Vector2		m_sca = { 1.0f,1.0f };				//スプライトの拡大率
	Vector2		m_spritePosition = Vector2::Zero;	//スプライトの位置


	float	m_maxHP = 0.0f;		//最大体力
	float	m_HP = 0.0f;		//現在の体力

	bool m_isInit = false;		//Initしてるかどうか
	bool m_isIn = true;
};

