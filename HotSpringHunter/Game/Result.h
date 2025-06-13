#pragma once
class Title;

//ゲームクリアリザルト
class Result :public IGameObject
{
public:
	Result();
	~Result();
	bool Start()override;
	void Update()override;
	void SwitchTitle();
	void Evaluation(); //評価
	void Score();	   //スコア
	void ClearTime();  //クリアタイム
	void NextButton();
	void Render(RenderContext& rc)override;

private:
	SpriteRender m_clearResultModel;
	FontRender m_evaluationFontRen;
	FontRender m_scoreFontRen;
	FontRender m_clearTimeFontRen;
	FontRender m_nextButtonRen;

	Title* m_title = nullptr;

	Vector2 m_nextButtonSize = Vector2(1.0f, 1.0f);

	int m_evaluation = 0;
	int m_finelScore = 0;
	float m_gameClearTime = 0.0f;
	Vector2 m_nextButtonColor = Vector2(1.0f, 0.0f);
	float m_nextButtonElapsed = 0.0f; //
	float m_buttonColor = 1.0f;

	bool m_isMaxTime = true;
};

//ゲームオーバーリザルト
class GameOverResult : public IGameObject
{
public:
	GameOverResult();
	~GameOverResult();

	bool Start() override;
	void Update() override;
	void OverSwitchTitle();
	void OverEvaluation(); //評価
	void OverScore();	   //スコア
	void OverTime();  //クリアタイム
	void NextButton();
	void Render(RenderContext& rc) override;

private:
	SpriteRender m_overResultModel;
	FontRender m_overEvaluationFontRen;
	FontRender m_overScoreFontRen;
	FontRender m_overoverTimeFontRen;
	FontRender m_overNextButtonRen;

	Title* m_title = nullptr;

	Vector2 m_nextButtonSize = Vector2(1.0f, 1.0f);

	int m_evaluation = 0;
	int m_finelScore = 0;
	float m_gameClearTime = 0.0f;
	Vector2 m_nextButtonColor = Vector2(1.0f, 0.0f);
	float m_nextButtonElapsed = 0.0f; //
	float m_buttonColor = 1.0f;

	bool m_isMaxTime = true;
};

