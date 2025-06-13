#include "stdafx.h"
#include "FenceManager.h"
#include "BackGround/MapObject/Fence.h"
#include "BackGround/HitObject/HItBoxFence.h"

FenceManager::FenceManager()
{

}
FenceManager::~FenceManager()
{

}

bool FenceManager::Start()
{
	m_fence			= FindGO<Fence>("fence");
	m_hitBoxFence	= FindGO<HItBoxFence>("hitBoxFence");

	return true;
}

void FenceManager::Update()
{

}

/// <summary>
/// 柵をアクティブ状態に遷移させます。
/// </summary>
void FenceManager::toFenceActive()
{
	//柵を描画状態へ
	m_isDrawFence = true;
	//柵の当たり判定を有効に
	m_isHitActive = true;
}

/// <summary>
/// 柵を非アクティブ状態に設定します。
/// </summary>
void FenceManager::toFenceInactive()
{
	//柵を非描画状態へ
	m_isDrawFence = false;
	//柵の当たり判定を無効に
	m_isHitActive = false;
}