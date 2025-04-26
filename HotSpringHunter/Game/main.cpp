#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include"Title.h"


// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//Titleクラスのオブジェクトを作成。
	NewGO<Title>(0, "title");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		TMEngine::GetInstance()->Execute();
	}

	TMEngine::DeleteInstance();

	//delete g_k2EngineLow;

	return 0;
}

