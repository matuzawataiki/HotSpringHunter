#pragma once
class Title;
class SceneManager;

//ゲームクリアリザルト
class Result :public IGameObject
{
public:
	Result();
	~Result();
	bool Start()override;
	void Update()override;

	void SwitchTitle();
	void ClearTime();  //クリアタイム
	void NextButton();
	void Render(RenderContext& rc)override;

	float m_gameTime = 0.0f;

private:
	SceneManager* m_sceneManager = nullptr;

	SpriteRender m_clearResultModel;
	SpriteRender m_scoreFont;
	SpriteRender m_timeFont;
	SpriteRender m_rankS; //scoreS/A/B/C
	SpriteRender m_rankA;
	SpriteRender m_rankB;
	SpriteRender m_rankC;

	FontRender m_gameTimeFontRen;
	FontRender m_nextButtonRen;

	Title* m_title = nullptr;

	Vector2 m_nextButtonSize  = Vector2(1.0f, 1.0f);
	Vector2 m_nextButtonColor = Vector2(1.0f, 0.0f);

	int m_finelScore = 0;

	float m_nextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor       = 1.0f;

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
	void OverTime();  //クリアタイム
	void NextButton();
	void Render(RenderContext& rc) override;

	float m_gameTime = 0.0f;

private:
	SceneManager* m_sceneManager = nullptr;

	SpriteRender m_overResultModel;
	SpriteRender m_scoreFont;
	SpriteRender m_timeFont;
	SpriteRender m_rankS; //scoreS/A/B/C
	SpriteRender m_rankA;
	SpriteRender m_rankB;
	SpriteRender m_rankC;

	FontRender m_gameOverTimeFontRen;
	FontRender m_overNextButtonRen;

	Title* m_title = nullptr;

	Vector2 m_nextButtonSize  = Vector2(1.0f, 1.0f);
	Vector2 m_nextButtonColor = Vector2(1.0f, 0.0f);

	int m_finelScore = 0;

	float m_nextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor       = 1.0f;
	
	bool m_isMaxTime = true;
};

