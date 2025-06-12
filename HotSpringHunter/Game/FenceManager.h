#pragma once
class Fence;
class HItBoxFence;
class FenceManager:public IGameObject
{
public:
	FenceManager();
	~FenceManager();
	bool Start()override;
	//柵をアクティブにする
	void toFenceActive();
	//柵を非アクティブにする
	void toFenceInactive();
	void Update()override;

	//ゲッター
	//柵描画状態を取得
	bool GetIsDrawFence() const { return m_isDrawFence; };
	//当たり判定状態を取得
	bool GetIsHitActive() const { return m_isHitActive; };

private:
	Fence*			m_fence			= nullptr;
	HItBoxFence*	m_hitBoxFence	= nullptr;

	bool			m_isDrawFence	= false;				//柵を描画するか
	bool			m_isHitActive	= false;				//当たり判定を有効にするか
};