#include "stdafx.h"
#include "GameClear.h"
#include "Game.h"
#include "Result.h"
#include "SmokeEmitter.h"

namespace
{
	const Vector2 GAME_CLEAR_SCALE_BASE = Vector2(1.0f, 1.0f);
	const Vector2 GAME_CLEAR_SCALE_MAX = Vector2(5.0f, 5.0f);

	//何秒かけておっきくする
	const float GAME_CLEAR_SCALE_TIME = 1.5f;
	//埋めつくす時間
	const float GAME_CLEAR_SMOKE_TIME = 5.0f;
	//消える時間
	const float DELETE_SMOKE_TIME = 6.0f;
}


GameClear::GameClear()
{
}

GameClear::~GameClear()
{
	DeleteGO(this);

}

bool GameClear::Start()
{

	m_gameClearModel = new SpriteRender();
	m_gameClearModel->Init("Assets/modelData/image/gameclear.dds", 384.0f, 216.0f);
	m_gameClearSize = GAME_CLEAR_SCALE_BASE;
	m_gameClearModel->SetScale(m_gameClearSize);



	return true;
}

void GameClear::Update()
{
	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	switch (m_resultState)
	{
		//ゲームクリアの文字を表示
	case enGameClear:
		//時間経過でmin~maxまでサイズが大きくなる
		m_elapsedTime += deltaTime;
		m_gameClearSize.Lerp(m_elapsedTime / GAME_CLEAR_SCALE_TIME, GAME_CLEAR_SCALE_BASE, GAME_CLEAR_SCALE_MAX);
		m_gameClearModel->SetScale(m_gameClearSize);
		m_gameClearModel->Update();
		//maxサイズになったらステート移行
		if (m_gameClearSize.x >= GAME_CLEAR_SCALE_MAX.x)
		{
			//経過時間を使いまわしたいので0秒にする
			m_elapsedTime = 0.0f;
			m_resultState = enSmokeStep1;
		}
		break;

		//湯気の生成
	case enSmokeStep1:
		m_smokeEmitter = NewGO<SmokeEmitter>(0);
		m_resultState = enSmokeStep2;
		break;

	case enSmokeStep2:
		m_elapsedTime += deltaTime;
		//湯気が画面を埋めつくしたら
		if (m_elapsedTime >= GAME_CLEAR_SMOKE_TIME)
		{
				m_resultState = enSmokeStep3;
		}
		break;

	case enSmokeStep3:
		if (m_resultModel == nullptr)
		{
			m_resultModel = new SpriteRender();
			m_resultModel->Init("Assets/modelData/image/gameclear_result.dds", 1920.0f, 1080.0f);
			delete m_gameClearModel;
			m_gameClearModel = nullptr;
			m_smokeEmitter->DeactiveEmitter();

		}
		m_elapsedTime = 0.0f;
		m_resultState = enSmokeStep4;
		break;

	case enSmokeStep4:
		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= DELETE_SMOKE_TIME)
		{
			m_resultState = enResult;
		}
		break;

	case enResult:

		m_clearResultModel = NewGO<Result>(0, "Result");
		Game* game = FindGO<Game>("game");
		DeleteGO(this);
		DeleteGO(game);
		DeleteGO(m_smokeEmitter);
		delete m_resultModel;
		m_resultModel = nullptr;
		break;
	}
}

void GameClear::Render(RenderContext&rc)
{
	if (m_resultModel != nullptr)
	{
		m_resultModel->Draw(rc);
	}
	if (m_gameClearModel != nullptr)
	{
		m_gameClearModel->Draw(rc);
	}
	

}

