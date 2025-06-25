#pragma once
class Game;
class GameRule;
class SoundEffect;

class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start()override;
	void Update();
	void SwitchGame();
	void NextButton();
	void PlayEffect();
	void Render(RenderContext& rc)override;

private:
	GameRule* m_gameRule       = nullptr;
	SoundEffect* m_soundEffect = nullptr; //BGM

	SpriteRender m_titleModel;

	FontRender m_titleNextButtonRen;

	Vector2 m_titleNextButtonColor = Vector2(1.0f, 0.0f);

	float m_titleNextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor            = 1.0f;

	bool m_isMaxTime = true;
	
};

