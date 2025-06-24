#pragma once
class Game;
class GameRule;

class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update();
	void SwitchGame();
	void NextButton();
	void Render(RenderContext& rc)override;

private:
	GameRule* m_gameRule = nullptr;

	SpriteRender m_titleModel;

	FontRender m_titleNextButtonRen;

	Vector2 m_titleNextButtonColor = Vector2(1.0f, 0.0f);

	float m_titleNextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor            = 1.0f;

	bool m_isMaxTime = true;
	
};

