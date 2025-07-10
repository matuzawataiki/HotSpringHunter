#include "stdafx.h"
#include "SceneManager.h"
#include "Title.h"
#include "TipsScene.h"
#include "Game.h"
#include "Result.h"

SceneManager* SceneManager::m_instance = nullptr;	// 初期化

SceneManager::SceneManager()
{
	// タイトルシーン追加
	AddSceneMap<Title>();
	// タイトルシーン追加
	AddSceneMap<TipsScene>();
	// タイトルシーン追加
	AddSceneMap<Game>();
	// タイトルシーン追加
	AddSceneMap<Result>();
	// ランキング画面追加
	AddSceneMap<ResultRanking>();
	// タイトルシーン追加
	AddSceneMap<GameOverResult>();
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	if (m_currentScene) {
		uint32_t nextSceneId;
		m_currentScene->Update();
		if (m_currentScene->RequestScene(nextSceneId)) {
			delete m_currentScene;
			CreateScene(nextSceneId);
		}
	}
}

void SceneManager::Render(RenderContext& rc)
{
	if (m_currentScene) {
		m_currentScene->Render(rc);
	}
}

void SceneManager::CreateScene(const uint32_t id)
{
	auto it = m_sceneMap.find(id);
	if (it == m_sceneMap.end()) {
		K2_ASSERT(false, "新規シーンが追加されていません。\n");
	}
	auto& createSceneFunc = it->second;
	m_currentScene = createSceneFunc();
	m_currentScene->Start();
}




SceneManagerObject::SceneManagerObject()
{
	SceneManager::CreateInstance();
}

SceneManagerObject::~SceneManagerObject()
{
	SceneManager::DestroyInstance();
}

bool SceneManagerObject::Start()
{
	// 最初のシーンを設定
	SceneManager::GetInstance()->CreateScene(Title::ID());
	return true;
}

void SceneManagerObject::Update()
{
	SceneManager::GetInstance()->Update();
}

void SceneManagerObject::Render(RenderContext& rc)
{
	SceneManager::GetInstance()->Render(rc);
}