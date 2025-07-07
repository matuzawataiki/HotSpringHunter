#pragma once
#include "Scene/IScene.h"
#include <map>
#include <functional>

/// <summary>
/// シーン管理を処理するためのゲームオブジェクト
/// </summary>
class SceneManagerObject : public IGameObject
{
public:
	SceneManagerObject();
	~SceneManagerObject();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
};

/// <summary>
/// シーン管理クラス
/// シーン遷移だったりは、ここを介して行います
/// シングルトンの勉強用で実装してみました
/// </summary>
class SceneManager
{
	// このクラスであればprivateの内容を触っても問題ないためfriendにする
	// friend class の練習
	friend class SceneManagerObject;

private:
	SceneManager();
	~SceneManager(); //仮想デストラクタ:派生クラスのデストラクタが呼ばれるようにする

public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// Scene追加を簡易的にする関数
	/// </summary>
	template <typename T>
	void AddSceneMap()
	{
		m_sceneMap.emplace(T::ID(), []()
			{
				return new T();
			});
	}
	/// <summary>
	/// シーン生成ラッパー関数
	/// </summary>
	void CreateScene(const uint32_t id);

private:
	using SceneMap = std::map<uint32_t, std::function<IScene*()>>;
	SceneMap m_sceneMap;		//シーンのマップ

	IScene* m_currentScene;		//現在のシーン
	



	/// <summary>
	/// シングルトン用
	/// </summary>
public:
	/// <summary>
	/// シングルトンインスタンスを生成する
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new SceneManager();
		}
	}

	/// <summary>
	/// シングルトンインスタンスを取得する
	/// </summary>
	static SceneManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// シングルトンインスタンスを破棄する
	/// </summary>
	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

private:
	static SceneManager* m_instance; //シングルトンインスタンス
};