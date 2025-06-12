#include "stdafx.h"
#include "Result.h"
#include "Title.h"

namespace
{
	//スプライトでやりたいかも(コンテニューの表示)
	const float NEXT_BUTTON_COLOR_BASE = 1.0f;
	const float NEXT_BUTTON_COLOR_MAX =  0.0f;

	const float NEXT_BUTTON_COLOR_TIME = 1.5f;

	//文字position
	//評価
	const Vector3 EVALUATION_FONT_POS = Vector3{ -850.0f,400.0f,0.0f };
	//スコア
	const Vector3 SCORE_FONT_POS = Vector3{ -850.0f,100.0f,0.0f };
	//タイム
	const Vector3 CLEAR_TIME_POS = Vector3{ -850.0f,0.0f,0.0f };
	//ネクストボタン
	const Vector3 NEXT_BUTTON_POS = Vector3{ -100.0f,-400.0f,0.0f };
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
	m_clearResultModel.Init("Assets/modelData/image/gameclear_result.dds", 1920.0f, 1080.0f);

	return true;
}

void Result::Update()
{
	SwitchTitle();

	Evaluation();

	Score();

	ClearTime();

	NextButton();
}

void Result::SwitchTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA)) 
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void Result::Evaluation()
{
	wchar_t evaluation[256];
	swprintf_s(evaluation, 256, L"評価 : %d", int(m_evaluation));
	m_evaluationFontRen.SetText(evaluation);
	m_evaluationFontRen.SetPosition(EVALUATION_FONT_POS);
	m_evaluationFontRen.SetScale(2.5f);
	m_evaluationFontRen.SetColor(g_vec4Black);
}

//スコアの表示
void Result::Score()
{
	wchar_t score[256];
	swprintf_s(score, 256, L"スコア : %d", int(m_finelScore));

	m_scoreFontRen.SetText(score);
	m_scoreFontRen.SetPosition(SCORE_FONT_POS);
	m_scoreFontRen.SetScale(2.0f);
	m_scoreFontRen.SetColor(g_vec4Black);
}

void Result::ClearTime()
{
	wchar_t time[256];
	swprintf_s(time, 256, L"クリアタイム : %d", int(m_gameClearTime));

	m_clearTimeFontRen.SetText(time);
	m_clearTimeFontRen.SetPosition(CLEAR_TIME_POS);
	m_clearTimeFontRen.SetScale(2.0f);
	m_clearTimeFontRen.SetColor(g_vec4Black);
}

void Result::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_nextButtonRen.SetText(L"タイトルに戻る");
	m_nextButtonRen.SetPosition(NEXT_BUTTON_POS);
	m_nextButtonRen.SetScale(1.0f);

	//if (m_isMaxTime == true)
	//{
	//	m_nextButtonElapsed += buttonDeltaTime;
	//	m_nextButtonColor.Lerp(m_nextButtonElapsed /*/ NEXT_BUTTON_COLOR_TIME*/, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));
	//	if (m_nextButtonElapsed >= 1.0f)
	//	{
	//		m_isMaxTime = false;
	//	}
	//}
	//if (m_isMaxTime == false)
	//{
	//	m_nextButtonElapsed -= buttonDeltaTime;
	//	m_nextButtonColor.Lerp(m_nextButtonElapsed /NEXT_BUTTON_COLOR_TIME, Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f), Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f));
	//	if (m_nextButtonElapsed <= 0.0f)
	//	{
	//		m_isMaxTime = true;
	//	}
	//}

	//あたらしい
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
	m_nextButtonColor.Lerp(m_buttonColor, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));


	m_nextButtonRen.SetColor(0.0f, 0.0f, 0.0f, m_nextButtonColor.x);
}


void Result::Render(RenderContext& rc)
{
	m_clearResultModel.Draw(rc);
	m_evaluationFontRen.Draw(rc);
	m_scoreFontRen.Draw(rc);
	m_clearTimeFontRen.Draw(rc);
	m_nextButtonRen.Draw(rc);
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
	m_overResultModel.Init("Assets/modelData/image/gameover_risult.dds",1920.0f,1080.0f);

	return true;
}

void GameOverResult::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) 
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameOverResult::Render(RenderContext& rc)
{
	m_overResultModel.Draw(rc);
}