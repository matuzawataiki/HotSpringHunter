#pragma once
#include "UI/UIBase.h"

class EnemyDetectionUI : public UIBase
{
public:
	EnemyDetectionUI();
	~EnemyDetectionUI();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

public:
	/// <summary>
	/// エネミーの位置を設定
	/// </summary>
	/// <param name="targetPosition">エネミーの位置</param>
	inline void SetTargetPosition(const Vector3& targetPosition) { m_targetPosition = targetPosition; }

	/// <summary>
	/// プレイヤーの位置を設定
	/// </summary>
	/// <param name="basePosition">プレイヤーの位置</param>
	inline void SetBasePosition(const Vector3& basePosition) { m_basePosition = basePosition; }

private:
	SpriteRender m_triangleImage;								//三角形の画像		
	Quaternion m_triangleRotation = Quaternion::Identity;		//三角形の回転

	Vector3 m_targetPosition	= Vector3::Zero;				//エネミーの位置
	Vector3 m_basePosition		= Vector3::Zero;				//プレイヤーの位置
};

