#include "stdafx.h"
#include "system/system.h"
#include "SoundEffect.h"
#include "Scene/SceneManager.h"
#include "common/SaveData.h"


// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;


/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	SkyCube* skyCube = NewGO<SkyCube>(0, "skyCube");
	skyCube->SetType(enSkyCubeType_SunriseToon);
	skyCube->SetScale(2000.0f);

	//セーブデータ作成
	SaveData::CreateInstance();

	//シーン管理クラスのオブジェクトを作成。
	NewGO<SceneManagerObject>(0, "sceneManagerObject");

	// サウンド再生ラッパーを作成。
	NewGO<SoundEffect>(0, "soundEffect");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		TMEngine::GetInstance()->Execute();
	}

	TMEngine::DeleteInstance();

	//セーブデータ削除
	SaveData::DestroyInstance();

	//delete g_k2EngineLow;

	return 0;
}

