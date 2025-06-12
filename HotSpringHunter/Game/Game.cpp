#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "EnemyBase.h"
#include "EnemyManager.h"
#include "EnemyHPBar.h"
#include "StartWaveCollision.h"
#include "UI.h"
#include "BackGround/StageManager.h";
#include "SceneManager.h"
#include "SoundEffect.h"
#include "GameOver.h"
#include "GameClear.h"


Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_sceneManager);
	DeleteGO(m_stageManager);
	DeleteGO(m_player);
	DeleteGO(m_ui);
	DeleteGO(m_soundEffect);
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_player = NewGO<Character::Player>(0, "player");
	m_sceneManager = NewGO<SceneManager>(0, "sceneManager");
	m_stageManager = NewGO<StageManager>(0, "stageManager");
	
	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	m_enemyManager = NewGO<EnemyManager>(0, "enemyManager");
	//m_enemyHPBar = NewGO<EnemyHPBar>(0, "enemyHPBar");

	m_ui = NewGO<UI>(0, "ui");

	m_soundEffect = NewGO<SoundEffect>(0, "soundEffect");

	//当たり判定を可視化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (m_player->GetPlayerHP() == 0.0f)
	{
		if (m_gameOver == nullptr)
		{
			m_gameOver = NewGO<GameOver>(0, "gameOver");
		}
	}
}
