#pragma once

#include "Scene/IScene.h" //継承をする時はインクルードする必要がある

class SoundEffect;

class Title :public IScene
{
	appScene(Title);

public:
	Title();
	~Title();
	bool Start()override;
	void Update()override;
	void NextButton();
	void PlayEffect();
	void Render(RenderContext& rc)override;

	bool RequestScene(uint32_t& id) override;

private:
	SoundEffect* m_soundEffect = nullptr; //BGM

	SpriteRender m_titleModel;

	FontRender m_titleNextButtonRen;

	Vector2 m_titleNextButtonColor = Vector2(1.0f, 0.0f);

	float m_titleNextButtonElapsed = 0.0f; //経過時間
	float m_buttonColor            = 1.0f;

	bool m_isMaxTime = true;
	
};

