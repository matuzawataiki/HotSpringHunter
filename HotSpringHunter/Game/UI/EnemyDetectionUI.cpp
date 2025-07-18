#include "stdafx.h"
#include "EnemyDetectionUI.h"

namespace
{
	float const TARGET_DISTANCE = 200.0f; 
	float const TRIANGLE_SIZE_WIDTH = 100.0f;
	float const TRIANGLE_SIZE_HEIGHT = 100.0f;
}

EnemyDetectionUI::EnemyDetectionUI()
{

}

EnemyDetectionUI::~EnemyDetectionUI()
{

}

bool EnemyDetectionUI::Start()
{
	m_triangleImage.Init("Assets/modelData/UI/redTriangle.dds", TRIANGLE_SIZE_WIDTH, TRIANGLE_SIZE_HEIGHT);

	return true;
}

void EnemyDetectionUI::Update()
{
	// 敵の方向
	Vector3 direction = m_targetPosition - m_basePosition;
	direction.y = 0.0f; // Y軸方向の成分を無視
	direction.Normalize();
	// 前方向を基準とした角度を計算して回転
	Quaternion q;
	{
		Vector3 angleCompute = Vector3::Front;
		const float dot = angleCompute.Dot(direction);
		float angleRad = acosf(dot);
		const float crossY = angleCompute.x * direction.z - angleCompute.z * direction.x;
		if (crossY < 0.0f) {
			// 右側なので回転を反転
			angleRad *= -1.0f;
		}
		q.SetRotationDegZ(Math::RadToDeg(angleRad));
	}

	// 方向をターゲット距離にスケール
	direction.Scale(TARGET_DISTANCE);

	// スプライトレンダーに設定
	m_triangleImage.SetPosition(direction.x, direction.z);
	m_triangleImage.SetRotation(q);
	m_triangleImage.Update();
}

void EnemyDetectionUI::Render(RenderContext& rc)
{
	m_triangleImage.Draw(rc);
}