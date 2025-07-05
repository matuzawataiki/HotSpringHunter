#pragma once
#include "Scene/IScene.h"
#include <map>
#include <functional>

//基底クラス
class SceneManager
{
private:
	SceneManager();
	virtual ~SceneManager(); //仮想デストラクタ:派生クラスのデストラクタが呼ばれるようにする

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Render(RenderContext& rc);

	//virtual bool Start()   = 0; //純粋仮想関数:派生クラスで実装されてなかったらエラーが出る
	//virtual void Update() = 0;
	//virtual void Render(RenderContext& rc) = 0;

private:
	using SceneMap = std::map<uint32_t, std::function<IScene*(uint32_t)>>;
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

