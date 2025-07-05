#include "stdafx.h"
#include "SceneManager.h"
#include "Title.h"
#include "TipsScene.h"
#include "Game.h"
#include "Result.h"
#include "GameClear.h"
#include "GameOver.h"

namespace
{
}

SceneManager* SceneManager::m_instance = nullptr;	// 初期化

SceneManager::SceneManager()
{
	// タイトルシーン追加
	m_sceneMap.emplace(Title::ID(), [](uint32_t id)
		{
			return new Title();
		});
	// タイトルシーン追加
	m_sceneMap.emplace(TipsScene::ID(), [](uint32_t id)
		{
			return new TipsScene();
		});
	// タイトルシーン追加
	m_sceneMap.emplace(Game::ID(), [](uint32_t id)
		{
			return new Game();
		});
	// タイトルシーン追加
	m_sceneMap.emplace(Title::ID(), [](uint32_t id)
		{
			return new Title();
		});
	// タイトルシーン追加
	m_sceneMap.emplace(Title::ID(), [](uint32_t id)
		{
			return new Title();
		});
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	if (m_currentScene) {
		uint32_t nextSceneId;
		if (m_currentScene->RequestScene(nextSceneId)) {
			delete m_currentScene;
			auto it = m_sceneMap.find(nextSceneId);
			if (it == m_sceneMap.end()) {
				K2_ASSERT(false, "新規シーンが追加されていません。\n");
			}
			auto& createSceneFunc = it->second;
			m_currentScene = createSceneFunc(nextSceneId);
			m_currentScene->Start();
		}
		m_currentScene->Update();
	}
}

void SceneManager::Render(RenderContext& rc)
{
	if (m_currentScene) {
		m_currentScene->Render(rc);
	}
}