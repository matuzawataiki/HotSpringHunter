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

	void SetTargetPosition(const Vector3& targetPosition) { m_targetPosition = targetPosition; }
	void SetBasePosition(const Vector3& basePosition) { m_basePosition = basePosition; }

private:
	SpriteRender m_triangleImage;
	Quaternion m_triangleRotation = Quaternion::Identity;		//三角形の回転

	Vector3 m_targetPosition	= Vector3::Zero;				//エネミーの位置
	Vector3 m_basePosition		= Vector3::Zero;				//プレイヤーの位置
};

