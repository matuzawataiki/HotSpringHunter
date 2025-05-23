#include "stdafx.h"
#include "StartWaveCollision.h"
#include "collision/CollisionObject.h"
#include "Player.h"
#include "EnemySpawn.h"

StartWaveCollision::StartWaveCollision()
{

}

StartWaveCollision::~StartWaveCollision()
{

}

bool StartWaveCollision::Start()
{
	//プレイヤー
	m_player = FindGO<Player>("player");
	m_enemySpawn = NewGO<EnemySpawn>(0, "enemySpawn");

	MakeCollision();

	return true;
}

void StartWaveCollision::Update()
{
	ChangePos();
	MakeCollision();	
}

void StartWaveCollision::MakeCollision()
{
	//マップの切り替わりにおく
	if (m_collision->IsHit(m_player->m_playerCharaCon))
	{

		m_enemySpawn = NewGO<EnemySpawn>(0, "enemySpawn");


		SwitchState();

	}
	//m_waveWall = waveWall02;

	//m_waveWall = waveWall03;
}

void StartWaveCollision::SwitchState()
{
	switch (m_waveWall) {
	case waveWall01:
		m_waveWall = waveWall02;
		break;
	case waveWall02:
		m_enemySpawn->m_zPos = 6500.0f;
		m_waveWall = waveWall03;
		break;
	case waveWall03:
		m_waveWall = waveWall04;
		break;
	/*case waveWall04:
		m_waveWall = waveWall05;
		break;*/
	default: 
		break;
	}
}

void StartWaveCollision::ChangePos()
{

	//コリジョンオブジェクトを作成
	m_collision = NewGO<CollisionObject>(0, "m_stagePos");

	//横置きのボックス
	Vector3 collisionSizeA = Vector3{ 1500.0f,600.0f,500.0f };
	//縦置きのボックス
	Vector3 collisionSizeB = Vector3{ 500.0f,600.0f,1500.0f };


	switch (m_waveWall) 	{

		case waveWall01:
			
			m_collisionPos = { 0.0f,0.0f,6250.0f };

			//boxのコリジョンを作成
			m_collision->CreateBox(
				m_collisionPos,
				Quaternion::Identity,
				collisionSizeA
			);
			break;

		case waveWall02:
			m_collisionPos = { -6500.0f,0.0f,7500.0f };
			//boxのコリジョンを作成
			m_collision->CreateBox(
				m_collisionPos,
				Quaternion::Identity,
				collisionSizeB
			);
			break;

		//case waveWall03:
		//	m_collisionPos = { -7500.0f,0.0f,13750.0f };
		//	//boxのコリジョンを作成
		//	m_collision->CreateBox(
		//		m_collisionPos,
		//		Quaternion::Identity,
		//		collisionSizeA
		//	);
		//	break;
	}	
}

void StartWaveCollision::Render(RenderContext& rc)
{

}