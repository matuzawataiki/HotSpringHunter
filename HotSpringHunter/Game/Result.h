#pragma once

#include "Scene/IScene.h"

class BattleManager;
class SoundEffect;

// リザルトのベース
class ResultBase : public IScene
{
protected:
	enum EnScoreRankType
	{
		enScoreRankType_S,
		enScoreRankType_A,
		enScoreRankType_B,
		enScoreRankType_C,
		enScoreRankType_Max,
	};

public:
	ResultBase();
	~ResultBase();

	bool Start()override;
	void Update()override;
	bool RequestScene(uint32_t& id) override;

protected:
	SpriteRender m_resultModel;
	SpriteRender m_scoreFont;
	SpriteRender m_timeFont;
	SpriteRender m_rankList[enScoreRankType_Max];

	FontRender m_gameTimeFontRen;
	FontRender m_nextButtonRen;

	Vector2 m_nextButtonSize = Vector2(1.0f, 1.0f);
	Vector2 m_nextButtonColor = Vector2(1.0f, 0.0f);

	int m_finelScore = 0;

	float m_gameTime = 0.0f;
	float m_nextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor = 1.0f;

	bool m_isMaxTime = true;
};

//ゲームクリアリザルト
class Result :public ResultBase
{
	appScene(Result);

public:
	Result();
	~Result();
	bool Start()override;
	void Update()override;
	bool RequestScene(uint32_t& id) override;

	void ClearTime();  //クリアタイム
	void NextButton();
	void ClearEffect(); //SE
	void Render(RenderContext& rc)override;

private:
	BattleManager* m_battleManager = nullptr;
	SoundEffect*  m_clearEffect  = nullptr;
};

//リザルトランキングクラス
class ResultRanking : public ResultBase
{
	appScene(ResultRanking);

private:
	static const int MAX_FONT_NUM = 3; //フォントの最大数

public:
	ResultRanking();
	~ResultRanking();

	bool Start() override;
	void Update() override;
	bool RequestScene(uint32_t& id) override;
	void Render(RenderContext& rc) override;

	void NextButton(); //タイトルに戻るためのボタン

private:
	SpriteRender m_rankingSprite; //ランキングのスプライト
	FontRender m_rankingFont[MAX_FONT_NUM]; //ランキングのフォントの配列
};

//ゲームオーバーリザルト
class GameOverResult : public ResultBase
{
	appScene(GameOverResult);

public:
	GameOverResult();
	~GameOverResult();

	bool Start() override;
	void Update() override;
	bool RequestScene(uint32_t& id) override;

	void OverTime();  //クリアタイム
	void NextButton();
	void OverEffect(); //SE
	void Render(RenderContext& rc) override;

private:
	BattleManager* m_battleManager = nullptr;
	SoundEffect*  m_overEffect  = nullptr;
};

