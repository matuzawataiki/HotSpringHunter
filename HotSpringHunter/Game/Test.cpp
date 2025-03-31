#include "stdafx.h"
#include "Test.h"

Test::Test()
{
}

Test::~Test()
{
}

bool Test::Start()
{
	//m_testImage.Init("Assets/modelData/testImage.DDS", 281.0f, 281.0f);
	//m_testImage.SetPosition(470.0f, 240.0f);
	//m_testImage.Update();
	//m_testText.SetText(L"âiå©Í£");
	//m_testText.SetPosition(500.0f, 500.0f);

	m_pointLight.SetPosition(10.0f, 10.0f, 0.0f);
	m_pointLight.SetColor(1.0f, 0.0f, 0.0f);
	m_pointLight.SetRange(400.0f);

	m_spotLight.Init(
		Vector3(10.0f, 10.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		10000.0f,
		Vector3(-1.0f, -1.0f, -1.0f),
		Math::DegToRad(90.0f)
	);

	m_hemisphereLight.Init(
		Vector3(0.5f, 0.5f, 0.3f),
		Vector3(0.15f, 0.7f, 0.95),
		Vector3(0.0f, 1.0f, 0.0f)
	);

	return true;
}

void Test::Update()
{

}

void Test::Render(RenderContext& rc)
{
	//m_testImage.Drow(rc);
	//m_testText.Draw(rc);
}
