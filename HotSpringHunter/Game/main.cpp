#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include"Title.h"
#include "SoundEffect.h"


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


	//Titleクラスのオブジェクトを作成。
	NewGO<Title>(0, "title");
	NewGO<SoundEffect>(0, "soundEffect");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		TMEngine::GetInstance()->Execute();
	}

	TMEngine::DeleteInstance();

	//delete g_k2EngineLow;

	return 0;
}

