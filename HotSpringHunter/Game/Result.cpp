#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "SceneManager.h"

namespace
{
	//透明度の最小値と最大値
	const float NEXT_BUTTON_COLOR_BASE = 1.0f;
	const float NEXT_BUTTON_COLOR_MAX  = 0.0f;
	//透明度変化の時間
	const float NEXT_BUTTON_COLOR_TIME = 1.5f;

	//　S/A/B/C評価 //
	//スプライトレンダーの横幅
	const float EVALUNATION_WIDTH        = 400.0f;
	//スプライトレンダーの縦幅
	const float EVALUATION_HEIGHT        = 280.0f;
	//スプライトレンダーのpos
	const Vector2 EVALUATION_SPRITE_POS  = Vector2{ -300.0f,250.0f };
	
	// スコア //
	//スプライトレンダーの横幅
	const float SCORE_WIDTH              = 300.0f;
	//スプライトレンダーの縦幅
	const float SCORE_HEIGHT             = 210.0f;
	//スプライトレンダーのpos
	const Vector2 SCORE_SPRITE_POS       = Vector2{ -700.0f,250.0f };

	// タイム //
	//フォントレンダーのpos
	const Vector3 CLEAR_TIME_POS         = Vector3{ -400.0f,-50.0f,0.0f };
	//スプライトレンダーの横幅
	const float CLEAR_TIME_WIDTH         = 300.0f;
	//スプライトレンダーの縦幅
	const float CLEAR_TIME_HEIGHT        = 100.0f;
	//スプライトレンダーのpos
	const Vector2 ECLEAR_TIME_SPRITE_POS = Vector2{ -700.0f,-85.0f };

	// ネクストボタン //
	const Vector3 NEXT_BUTTON_POS        = Vector3{ -750.0f,-400.0f,0.0f };
}

/// <summary>
/// ゲームクリアリザルトの表示
/// </summary>
Result::Result()
{
}

Result::~Result()
{

}

bool Result::Start()
{
	//リザルト
	m_clearResultModel.Init("Assets/modelData/image/gameclear_result.dds", 1920.0f, 1080.0f);

	//スコア
	m_scoreFont.Init("Assets/modelData/image/score.dds", SCORE_WIDTH, SCORE_HEIGHT);
	m_scoreFont.SetPosition(SCORE_SPRITE_POS);
	m_scoreFont.Update();

	//クリアタイム
	m_timeFont.Init("Assets/modelData/image/cleartime.dds", CLEAR_TIME_WIDTH, CLEAR_TIME_HEIGHT);
	m_timeFont.SetPosition(ECLEAR_TIME_SPRITE_POS);
	m_timeFont.Update();

	//スコアランクS
	m_rankS.Init("Assets/modelData/image/scores.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankS.SetPosition(EVALUATION_SPRITE_POS);
	m_rankS.Update();

	//スコアランクA
	m_rankA.Init("Assets/modelData/image/scorea.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankA.SetPosition(EVALUATION_SPRITE_POS);
	m_rankA.Update();

	//スコアランクB
	m_rankB.Init("Assets/modelData/image/scoreb.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankB.SetPosition(EVALUATION_SPRITE_POS);
	m_rankB.Update();

	//スコアランクC
	m_rankC.Init("Assets/modelData/image/scorec.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankC.SetPosition(EVALUATION_SPRITE_POS);
	m_rankC.Update();

	m_sceneManager = FindGO<SceneManager>("sceneManager");

	return true;
}

void Result::Update()
{
	//タイトル画面に戻る
	SwitchTitle();
	//クリアタイム
	ClearTime();
	//「タイトルに戻る」の表示処理
	NextButton();
}

//タイトル画面に戻る
void Result::SwitchTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

//クリアタイム
void Result::ClearTime()
{
	wchar_t time[256];
	swprintf_s(time, 256, L"%d", int(m_gameTime));

	m_gameTimeFontRen.SetText(time);
	m_gameTimeFontRen.SetPosition(CLEAR_TIME_POS);
	m_gameTimeFontRen.SetScale(2.0f);
	m_gameTimeFontRen.SetColor(g_vec4Black);
}

//「タイトルに戻る」の表示処理
void Result::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_nextButtonRen.SetText(L"PLESS A BUTTON");
	m_nextButtonRen.SetPosition(NEXT_BUTTON_POS);
	m_nextButtonRen.SetScale(1.0f);

	//文字の透明度を時間で変える
	m_nextButtonElapsed += buttonDeltaTime;
	const float NEXT_BUTTON_COLOR_PARCENT = m_nextButtonElapsed / NEXT_BUTTON_COLOR_TIME;
	if (m_isMaxTime == true)
	{
		m_buttonColor = 1.0f - NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor <= 0.0f)
		{
			m_buttonColor = 0.0f;
			m_nextButtonElapsed = 0.0f;
			m_isMaxTime = false;
		}
	}
	else
	{
		m_buttonColor = NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor >= 1.0f)
		{
			m_buttonColor = 1.0f;
			m_nextButtonElapsed = 0.0f;
			m_isMaxTime = true;
		}
	}
	//Lerp関数で滑らかに透明度を変化させる
	m_nextButtonColor.Lerp(m_buttonColor, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));
	m_nextButtonRen.SetColor(0.0f, 0.0f, 0.0f, m_nextButtonColor.x);
}


void Result::Render(RenderContext& rc)
{
	m_clearResultModel.Draw(rc);

	m_scoreFont.Draw(rc);
	m_timeFont.Draw(rc);

	m_gameTimeFontRen.Draw(rc);
	m_nextButtonRen.Draw(rc);

	//ランクの表示
	/*if (m_gameTime)
	{
		m_rankS.Draw(rc);
	}
	if(m_gameTime)
	{
		m_rankA.Draw(rc);
	}
	if (m_gameTime)
	{
		m_rankB.Draw(rc);
	}
	else
	{
		m_rankC.Draw(rc);
	}*/

	m_rankS.Draw(rc);
}

/// <summary>
/// ゲームオーバーリザルトの表示
/// </summary>

GameOverResult::GameOverResult()
{
}

GameOverResult::~GameOverResult()
{
}

bool GameOverResult::Start()
{
	//リザルト
	m_overResultModel.Init("Assets/modelData/image/gameover_risult.dds", 1920.0f, 1080.0f);

	//スコア
	m_scoreFont.Init("Assets/modelData/image/score.dds", SCORE_WIDTH, SCORE_HEIGHT);
	m_scoreFont.SetPosition(SCORE_SPRITE_POS);
	m_scoreFont.Update();

	//クリアタイム
	m_timeFont.Init("Assets/modelData/image/cleartime.dds", CLEAR_TIME_WIDTH, CLEAR_TIME_HEIGHT);
	m_timeFont.SetPosition(ECLEAR_TIME_SPRITE_POS);
	m_timeFont.Update();

	//スコアランクS
	m_rankS.Init("Assets/modelData/image/scores.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankS.SetPosition(EVALUATION_SPRITE_POS);
	m_rankS.Update();

	//スコアランクA
	m_rankA.Init("Assets/modelData/image/scorea.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankA.SetPosition(EVALUATION_SPRITE_POS);
	m_rankA.Update();

	//スコアランクB
	m_rankB.Init("Assets/modelData/image/scoreb.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankB.SetPosition(EVALUATION_SPRITE_POS);
	m_rankB.Update();

	//スコアランクC
	m_rankC.Init("Assets/modelData/image/scorec.dds", EVALUNATION_WIDTH, EVALUATION_HEIGHT);
	m_rankC.SetPosition(EVALUATION_SPRITE_POS);
	m_rankC.Update();

	m_sceneManager = FindGO<SceneManager>("sceneManager");

	return true;
}

void GameOverResult::Update()
{
	//タイトル画面に戻る
	OverSwitchTitle();
	//ゲームオーバータイム
	OverTime();
	//「タイトルに戻る」の表示処理
	NextButton();
}

//タイトル画面に戻る
void GameOverResult::OverSwitchTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

//ゲームオーバータイム
void GameOverResult::OverTime()
{
	wchar_t time[256];
	swprintf_s(time, 256, L"%d", int(m_gameTime));

	m_gameOverTimeFontRen.SetText(time);
	m_gameOverTimeFontRen.SetPosition(CLEAR_TIME_POS);
	m_gameOverTimeFontRen.SetScale(2.0f);
	m_gameOverTimeFontRen.SetColor(g_vec4Black);
}

//「タイトルに戻る」の表示処理
void GameOverResult::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_overNextButtonRen.SetText(L"PLESS A BUTTON");
	m_overNextButtonRen.SetPosition(NEXT_BUTTON_POS);
	m_overNextButtonRen.SetScale(1.0f);

	//文字の透明度を時間で変える
	m_nextButtonElapsed += buttonDeltaTime;
	const float NEXT_BUTTON_COLOR_PARCENT = m_nextButtonElapsed / NEXT_BUTTON_COLOR_TIME;
	if (m_isMaxTime == true)
	{
		m_buttonColor = 1.0f - NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor <= 0.0f)
		{
			m_buttonColor = 0.0f;
			m_nextButtonElapsed = 0.0f;
			m_isMaxTime = false;
		}
	}
	else
	{
		m_buttonColor = NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor >= 1.0f)
		{
			m_buttonColor = 1.0f;
			m_nextButtonElapsed = 0.0f;
			m_isMaxTime = true;
		}
	}
	//Lerp関数で滑らかに透明度を変化させる
	m_nextButtonColor.Lerp(m_buttonColor, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));
	m_overNextButtonRen.SetColor(0.0f, 0.0f, 0.0f, m_nextButtonColor.x);
}

void GameOverResult::Render(RenderContext& rc)
{
	m_overResultModel.Draw(rc);

	m_scoreFont.Draw(rc);
	m_timeFont.Draw(rc);

	m_gameOverTimeFontRen.Draw(rc);
	m_overNextButtonRen.Draw(rc);

	//ランクの表示
/*if (m_gameTime)
{
	m_rankS.Draw(rc);
}
if(m_gameTime)
{
	m_rankA.Draw(rc);
}
if (m_gameTime)
{
	m_rankB.Draw(rc);
}
else
{
	m_rankC.Draw(rc);
}*/

	m_rankS.Draw(rc);
}