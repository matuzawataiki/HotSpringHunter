#pragma once

class Player;
class EnemySpawn;

class StartWaveCollision : public IGameObject
{
public:
	StartWaveCollision();
	~StartWaveCollision();
	bool Start()override;
	void Update()override;
	void MakeCollision();
	void ChangePos();
	void SwitchState();
	void Render(RenderContext& rc);

	Player* m_player;
	EnemySpawn* m_enemySpawn;

private:
	CollisionObject* m_collision = nullptr;

	Vector3 m_collisionPos = Vector3::Zero;		
	Quaternion m_boxRot;
	int m_waveWall = 0;

	//‹N“®’†‚©
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

