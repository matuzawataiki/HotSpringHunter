#include "stdafx.h"
#include "Title.h"
#include "Scene/TipsScene.h"
#include "SoundEffect.h"

namespace
{
	//文字の透明度
	const float NEXT_BUTTON_COLOR_BASE = 1.0f;
	const float NEXT_BUTTON_COLOR_MAX  = 0.0f;

	const float NEXT_BUTTON_COLOR_TIME = 1.5f;

	//文字position
	const Vector3 NEXT_BUTTON_POS      = Vector3{ -280.0,-300.0f,0.0f };
}

Title::Title()
{
}

Title::~Title()
{
	
}

bool Title::Start()
{
	m_titleModel.Init("Assets/modelData/image/newtitle.dds", 1920.0f, 1080.0f);

	//BGM
	m_soundEffect = FindGO<SoundEffect>("soundEffect");

	PlayEffect();

	return true;
}

void Title::Update()
{
	NextButton();
}

void Title::NextButton()
{
	const float buttonDeltaTime = g_gameTime->GetFrameDeltaTime();

	m_titleNextButtonRen.SetText(L"PLESS A BUTTON");
	m_titleNextButtonRen.SetPosition(NEXT_BUTTON_POS);
	m_titleNextButtonRen.SetScale(1.5f);

	//文字の透明度を時間で変える
	m_titleNextButtonElapsed += buttonDeltaTime;
	//定数に計算式を入れる
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
	//lerpで透明度を滑らかに表示
	m_titleNextButtonColor.Lerp(m_buttonColor, Vector2(NEXT_BUTTON_COLOR_BASE, 0.0f), Vector2(NEXT_BUTTON_COLOR_MAX, 0.0f));
	m_titleNextButtonRen.SetColor(0.0f, 0.0f, 0.0f, m_titleNextButtonColor.x);
}

void Title::PlayEffect()
{
	//BGMの再生
	m_soundEffect->Play(enTitleBGM);
}

void Title::Render(RenderContext& rc)
{
	m_titleModel.Draw(rc);
	m_titleNextButtonRen.Draw(rc);
}

bool Title::RequestScene(uint32_t& id)
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		id = TipsScene::ID();
		return true;
	}
	return false;
}