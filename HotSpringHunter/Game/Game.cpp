#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	m_playerModel.Init("Assets/modelData/unityChan.tkm");
	m_mapModel.Init("Assets/modelData/bg.tkm");
	//m_testImage.Init("Assets/modelData/testImage.DDS", 281.0f, 281.0f);
	//m_testImage.SetPosition(470.0f, 240.0f);
	//m_testImage.Update();
	//m_testText.SetText(L"‰iŒ©ê£");
	//m_testText.SetPosition(500.0f, 500.0f);
	g_camera3D->SetPosition(m_cameraPos);

	return true;
}

void Game::Update()
{
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_cameraPos.x -= 50.0f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_cameraPos.x += 50.0f;
	}
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_cameraPos.z += 50.0f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_cameraPos.x -= 50.0f;
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_cameraPos = Vector3(0.0f, 500.0f, -1000.0f);
	}

	g_camera3D->SetPosition(m_cameraPos);

	m_playerModel.Update();
	m_mapModel.Update();

}

void Game::Render(RenderContext& rc)
{
	m_playerModel.Draw(rc);
	m_mapModel.Draw(rc);
	//m_testImage.Drow(rc);
	//m_testText.Draw(rc);
}
