#include "stdafx.h"
#include "BreakedRock.h"

namespace
{
	// 加速度
	const Vector3 ACCELERATION = Vector3(0.0f, -9.8f, 0.0f);

	int GetRand()
	{
		std::srand(std::time(nullptr) + std::rand());
		return std::rand();
	}

	// 与える力を取得
	Vector3 GetForce()
	{
		// 現在の時間をseed値にして、良い感じのランダムになるようにする
		float angle = static_cast<float>(GetRand() % 360);
		// 求めた角度を使ってY軸回転する
		// 岩が飛ぶ方向を求める
		Vector3 direction = Vector3::Front;
		Quaternion q;
		q.SetRotationY(angle);
		q.Apply(direction);
		// 求めた方向を使って飛ぶ力を与える
		Vector3 force = direction;
		force.y = 1.0f;
		// ここを調整
		constexpr float POWER_XZ = 4.0f;
		constexpr float POWER_Y = 6.0f;
		force.x *= POWER_XZ;
		force.y *= POWER_Y;
		force.z *= POWER_XZ;
		return force;
	}
}

BreakedRock::BreakedRock()
{
}

BreakedRock::~BreakedRock()
{
}

bool BreakedRock::Start()
{
	m_breakedRock.Init("Assets/modelData/bear/slowStone.tkm");
	m_breakedRock.SetPosition(m_position);
	m_breakedRock.SetScale(Vector3{ 0.5f,0.3f,0.5f });

	Quaternion q;
	q.SetRotationX(static_cast<float>(GetRand() % 360));
	q.SetRotationY(static_cast<float>(GetRand() % 360));
	q.SetRotationZ(static_cast<float>(GetRand() % 360));

	m_breakedRock.SetRotation(q);

	m_velocity = GetForce();
	return true;
}

void BreakedRock::Update()
{
	// 加速を求めて座標に設定
	// 最初に飛ぶ力を与えて徐々に落ちていくような感じ
	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	m_velocity.x += (ACCELERATION.x * deltaTime);
	m_velocity.y += (ACCELERATION.y * deltaTime);
	m_velocity.z += (ACCELERATION.z * deltaTime);
	m_position.Add(m_velocity);

	m_breakedRock.SetPosition(m_position);
	m_breakedRock.Update();

	if (m_position.y < 0.0f) {
		DeleteGO(this);
	}
}

void BreakedRock::Render(RenderContext& rc)
{
	m_breakedRock.Draw(rc);
}
