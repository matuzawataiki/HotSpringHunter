#include "stdafx.h"
#include "Result.h"
#include "Scene/SceneManager.h"
#include "Title.h"
#include "BattleManager.h"
#include "SoundEffect.h"
#include "common/SaveData.h"

namespace
{
	//透明度の最小値と最大値
	const float NEXT_BUTTON_COLOR_BASE = 1.0f;
	const float NEXT_BUTTON_COLOR_MAX  = 0.0f;
	//透明度変化の時間
	const float NEXT_BUTTON_COLOR_TIME = 1.5f;

	//　S/A/B/C評価 //
	//スプライトレンダーの横幅
	const float EVALUNATION_WIDTH         = 400.0f;
	//スプライトレンダーの縦幅
	const float EVALUATION_HEIGHT         = 280.0f;
	//スプライトレンダーのpos
	const Vector2 EVALUATION_SPRITE_POS   = Vector2{ -300.0f,250.0f };
	
	// スコア //
	//スプライトレンダーの横幅
	const float SCORE_WIDTH               = 300.0f;
	//スプライトレンダーの縦幅
	const float SCORE_HEIGHT              = 210.0f;
	//スプライトレンダーのpos
	const Vector2 SCORE_SPRITE_POS        = Vector2{ -700.0f,250.0f };

	// タイム //
	//フォントレンダーのpos
	const Vector3 CLEAR_TIME_POS          = Vector3{ -400.0f,-50.0f,0.0f };
	//スプライトレンダーの横幅
	const float CLEAR_TIME_WIDTH          = 300.0f;
	//スプライトレンダーの縦幅
	const float CLEAR_TIME_HEIGHT         = 100.0f;
	//スプライトレンダーのpos
	const Vector2 ECLEAR_TIME_SPRITE_POS  = Vector2{ -700.0f,-85.0f };

	// ネクストボタン //
	const Vector3 NEXT_BUTTON_POS         = Vector3{ -750.0f,-400.0f,0.0f };
	//ランキングのネクストボタン
	const Vector3 RANKING_NEXT_BUTTON_POS = Vector3{ -200.0f,-400.0f,0.0f };

	//　ランキング //
	//フォントレンダーのposx
	const float RANKING_POSX              = -600.0f;
	//フォントレンダーのposy
	const float RANKING_POSY              = 225.0f;
	//フォントレンダーの文字間隔
	const float RANKING_POS_SPACE         = 240.0f;


	// リザルトに表示するスコアランクアイコン
	constexpr const char* SCORE_RANK_ICON_PATH[] =
	{
		"Assets/modelData/image/scores.dds",
		"Assets/modelData/image/scorea.dds",
		"Assets/modelData/image/scoreb.dds",
		"Assets/modelData/image/scorec.dds",
	};
}

/// <summary>
/// リザルト処理のベースクラス
/// </summary>
ResultBase::ResultBase()
{
}

ResultBase::~ResultBase()
{
}

bool ResultBase::Start()
{
	//スコア
	m_scoreFont.Init("Assets/modelData/image/score.dds", SCORE_WIDTH, SCORE_HEIGHT);
	m_scoreFont.SetPosition(SCORE_SPRITE_POS);
	m_scoreFont.Update();

	//クリアタイム
	m_timeFont.Init("Assets/modelData/image/cleartime.dds", CLEAR_TIME_WIDTH, CLEAR_TIME_HEIGHT);
	m_timeFont.SetPosition(ECLEAR_TIME_SPRITE_POS);
	m_timeFont.Update();

	// スコアランク
	for (int i = 0; i<ARRAYSIZE(SCORE_RANK_ICON_PATH); i++)
	{
		//スコアランクS
		m_rankList[i].Init(SCORE_RANK_ICON_PATH[i], EVALUNATION_WIDTH, EVALUATION_HEIGHT);
		m_rankList[i].SetPosition(EVALUATION_SPRITE_POS);
		m_rankList[i].Update();
	}

	return true;
}

void ResultBase::Update()
{
}

bool ResultBase::RequestScene(uint32_t& id)
{
	return true;
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
	ResultBase::Start();

	// リザルト表示用に時間を取得
	m_gameTime = SaveData::GetInstance()->GetResultTime();

	//リザルト
	m_resultModel.Init("Assets/modelData/image/gameclear_result.dds", 1920.0f, 1080.0f);

	m_battleManager = FindGO<BattleManager>("battleManager");
	m_clearEffect = FindGO<SoundEffect>("soundEffect");

	//ゲームクリアの音
	ClearEffect();

	return true;
}

void Result::Update()
{
	//クリアタイム
	ClearTime();
	//「タイトルに戻る」の表示処理
	NextButton();
}

bool Result::RequestScene(uint32_t& id)
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		id = ResultRanking::ID();
		return true;
	}
	return false;
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

void Result::ClearEffect()
{
	m_clearEffect->Play(enGameClearBGM,false);
}


void Result::Render(RenderContext& rc)
{
	m_resultModel.Draw(rc);

	m_scoreFont.Draw(rc);
	m_timeFont.Draw(rc);

	m_gameTimeFontRen.Draw(rc);
	m_nextButtonRen.Draw(rc);

	//ランクの表示
	if (m_gameTime <= 120.0f)
	{
		m_rankList[enScoreRankType_S].Draw(rc);
	}
	else if(m_gameTime <= 150.0f)
	{
		m_rankList[enScoreRankType_A].Draw(rc);
	}
	else if (m_gameTime <= 200.0f)
	{
		m_rankList[enScoreRankType_B].Draw(rc);
	}
	else
	{
		m_rankList[enScoreRankType_C].Draw(rc);
	}
}

///// <summary>
/// リザルトランキングの表示
/// </summary>
ResultRanking::ResultRanking()
{
}

ResultRanking::~ResultRanking()
{
}

bool ResultRanking::Start()
{
	//ランキングリザルト
	m_rankingSprite.Init("Assets/modelData/image/ranking.dds",1920.0f,1080.0f);

	//新記録
	m_newRecordSprite.Init("Assets/modelData/image/newrecord.dds", 205.0f, 85.0f);
	m_newRecordSprite.SetPosition(Vector2{ -200.0f,250.0f });
	Quaternion rot;
	rot.SetRotationZ(nsK2EngineLow::Math::DegToRad(-10.0f));
	m_newRecordSprite.SetRotation(rot);
	m_newRecordSprite.Update();

	return true;
}

void ResultRanking::Update()
{
	//フォントレンダーに指定した時間を設定する
	//ついでにpositionも設定する
	//ラムダ式を使ってみた(無名関数->汎用性を高める時に使う)
	auto setRankingText = [](FontRender* fontRender, const float rankingTime, const Vector3& position)
		{
			wchar_t time[256];
			swprintf_s(time, 256, L"%.03f", rankingTime);

			fontRender->SetText(time);
			fontRender->SetPosition(position);
			fontRender->SetScale(2.0f);
			fontRender->SetColor(g_vec4Black);
		};

	//ソートを使ってランキングの入れ替え(今回は時間が速いほうが1位になる)
	std::vector<float> rankingData = SaveData::GetInstance()->GetResultTimeList();
	std::sort(rankingData.begin(), rankingData.end(), [](int a, int b)
		{
			return a < b;
		});

	//ラムダ式を呼び出して設定してる
	//2行目にif文を1行にまとめている
	for (int i = 0; i < MAX_FONT_NUM; ++i) {
		const float t = i < rankingData.size() ? rankingData[i] : 10000.0f;
		setRankingText(&m_rankingFont[i], t, Vector3(RANKING_POSX, RANKING_POSY - (i * RANKING_POS_SPACE), 0.0f));
	}

	//タイトルにもどるボタンの表示処理
	NextButton();
}

bool ResultRanking::RequestScene(uint32_t& id)
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		id = Title::ID();
		return true;
	}

	return false;
}

void ResultRanking::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_nextButtonRen.SetText(L"PLESS A BUTTON");
	m_nextButtonRen.SetPosition(RANKING_NEXT_BUTTON_POS);
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

void ResultRanking::Render(RenderContext& rc)
{
	m_rankingSprite.Draw(rc);
	m_newRecordSprite.Draw(rc);

	m_nextButtonRen.Draw(rc);

	for (int i = 0; i < MAX_FONT_NUM; ++i) {
		m_rankingFont[i].Draw(rc);
	}
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
	ResultBase::Start();

	// リザルト表示用に時間を取得
	m_gameTime = SaveData::GetInstance()->GetResultTime();

	//リザルト
	m_resultModel.Init("Assets/modelData/image/gameover_risult.dds", 1920.0f, 1080.0f);

	m_battleManager = FindGO<BattleManager>("battleManager");
	m_overEffect = FindGO<SoundEffect>("soundEffect");

	//ゲームオーバーの音
	OverEffect();

	return true;
}

void GameOverResult::Update()
{
	//ゲームオーバータイム
	OverTime();
	//「タイトルに戻る」の表示処理
	NextButton();
}

bool GameOverResult::RequestScene(uint32_t& id)
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		id = Title::ID();
		return true;
	}
	return false;
}

//ゲームオーバータイム
void GameOverResult::OverTime()
{
	wchar_t time[256];
	swprintf_s(time, 256, L"%d", int(m_gameTime));

	m_gameTimeFontRen.SetText(time);
	m_gameTimeFontRen.SetPosition(CLEAR_TIME_POS);
	m_gameTimeFontRen.SetScale(2.0f);
	m_gameTimeFontRen.SetColor(g_vec4Black);
}

//「タイトルに戻る」の表示処理
void GameOverResult::NextButton()
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

void GameOverResult::OverEffect()
{
	m_overEffect->Play(enGameOverBGM,false);
}

void GameOverResult::Render(RenderContext& rc)
{
	m_resultModel.Draw(rc);

	m_scoreFont.Draw(rc);
	m_timeFont.Draw(rc);

	m_gameTimeFontRen.Draw(rc);
	m_nextButtonRen.Draw(rc);

	//ランクの表示
	m_rankList[enScoreRankType_C].Draw(rc);
}

