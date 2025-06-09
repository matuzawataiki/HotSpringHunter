#pragma once

namespace Character {
	class Player;
}
class EnemySpawn;

class StartWaveCollision : public IGameObject
{
public:
	StartWaveCollision();
	~StartWaveCollision();
	bool Start()override;
	void CreateBear();
	void Update()override;
	void MakeCollision();
	void ChangePos();
	void SwitchState();
	void Render(RenderContext& rc)override;

private:
	CollisionObject* m_collision = nullptr;
	Character::Player* m_player = nullptr;
	EnemySpawn* m_enemySpawn = nullptr;

	Vector3 m_collisionPos = Vector3::Zero;		
	Quaternion m_boxRot;
	int m_waveWall = 0;

	//起動中か
	enum
	{
		waveWall01,
		waveWall02,
		waveWall03,
		waveWall04,
		//waveWallNum,
	};
	int m_enemyState = waveWall01;
};

