#pragma once

namespace nsK2Engine
{
	class CollisionObject;
}

namespace Character {
	class Player;
}

class ProjectileManager;
class SlashAttack : public IGameObject
{
public:
	SlashAttack();
	~SlashAttack();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	enum EnPowerUp
	{
		enPowerUp1,
		enPowerUp2,
		enPowerUp3,
		enPowerUpNull,
	};

	void OnCharge() { m_isCharge = true; }
	void SetPowerUp(EnPowerUp powerUp) { m_powerUp = powerUp; }	

private:
	Character::Player*	m_player = nullptr;
	ProjectileManager*	m_projectileManager = nullptr;


	CollisionObject		m_mainCollision;		//メインのコライダー
	CollisionObject		m_subCollision[2];		//サブのコライダー
	ModelRender			m_sphereModel;			//メインのモデル
	ModelRender			m_subModel[2];			//サブのモデル

	Vector3 m_position = Vector3::Zero;			//玉の位置
	Vector3 m_targetPosition = Vector3::Zero;	//ターゲットの位置
	Vector3 m_moveDirection = Vector3::Zero;	//玉の移動する方向

	Vector3 m_rot[2];
	Vector3 m_vec[2];

	bool m_isCharge = false;
	int m_powerUp = enPowerUpNull;

};

