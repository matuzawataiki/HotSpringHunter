#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "GameRule.h"

namespace
{
	//文字の透明度
	const float NEXT_BUTTON_COLOR_BASE = 1.0f;
	const float NEXT_BUTTON_COLOR_MAX = 0.0f;

	const float NEXT_BUTTON_COLOR_TIME = 1.5f;

	//文字position
	const Vector3 NEXT_BUTTON_POS = Vector3{ -280.0,-300.0f,0.0f };
}

Title::Title()
{
	DeleteGO(m_gameRule);
}

Title::~Title()
{

}

bool Title::Start()
{
	m_titleModel.Init("Assets/modelData/image/newtitle.dds", 1920.0f, 1080.0f);

	return true;
}

void Title::Update()
{
	SwitchGame();
	NextButton();
}

void Title::SwitchGame()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_gameRule = NewGO<GameRule>(0, "GameRule");
		DeleteGO(this);
	}
}
void Title::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_titleNextButtonRen.SetText(L"Aボタンを押してね");
	m_titleNextButtonRen.SetPosition(NEXT_BUTTON_POS);
	m_titleNextButtonRen.SetScale(1.5f);

	//文字の透明度を変える
	m_titleNextButtonElapsed += buttonDeltaTime;
	const float TITLE_NEXT_BUTTON_COLOR_PARCENT = m_titleNextButtonElapsed / NEXT_BUTTON_COLOR_TIME;
	if (m_isMaxTime == true)
	{
		m_buttonColor = 1.0f - TITLE_NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor <= 0.0f)
		{
			m_buttonColor = 0.0f;
			m_titleNextButtonElapsed = 0.0f;
			m_isMaxTime = false;
		}
	}
	else
	{
		m_buttonColor = TITLE_NEXT_BUTTON_COLOR_PARCENT;
		if (m_buttonColor >= 1.0f)
		{
			m_buttonColor = 1.0f;
			m_titleNextButtonElapsed = 0.0f;
			m_isMaxTime = true;
		}
	}
	m_titleNextButtonColor.Lerp(m_buttonColor, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));
	m_titleNextButtonRen.SetColor(0.0f, 0.0f, 0.0f, m_titleNextButtonColor.x);
}

void Title::Render(RenderContext& rc)
{
	m_titleModel.Draw(rc);
	m_titleNextButtonRen.Draw(rc);
}