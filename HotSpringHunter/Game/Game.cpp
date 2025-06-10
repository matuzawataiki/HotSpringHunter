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
#include "Bear.h"
#include "WildBoar.h"
#include "SoundEffect.h"
#include "GameOver.h"
#include "GameClear.h"
#include "SnakeEnemy.h"


Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_stageManager);
	DeleteGO(m_player);
	DeleteGO(m_bear);
	DeleteGO(m_ui);
	DeleteGO(m_soundEffect);
}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
	m_stageManager = NewGO<StageManager>(0, "stageManager");
	m_player = NewGO<Character::Player>(0, "player");
	//m_waveCollision = NewGO<StartWaveCollision>(0, "startWaveCollision");
	//m_bear = NewGO<Bear>(0, "bear");
	//m_wildBoar = NewGO<WildBoar>(0, "wildBoar");

	//m_wildBoar = NewGO<WildBoar>(0, "wildBoar");
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
	//テスト確認用のクリア処理です
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		if (m_gameClear == nullptr)
		{
			m_gameClear = NewGO<GameClear>(0, "GameClear");
		}
	}
}
