#include "stdafx.h"
#include "CharacterBase.h"

//キャラクター基底クラス。

namespace {
	const float GRAVITY_AMOUNT = 10.0f;
}

CharacterBase::CharacterBase()
{
}

CharacterBase::~CharacterBase()
{
}

bool CharacterBase::Start()
{
	return true;
}

void CharacterBase::Update()
{
}

/// <summary>
/// キャラの基本移動（キャラの移動、重力）。
/// </summary>
/// <param name="charaCon"></param>当たり判定。
/// <param name="position"></param>座標。
/// <param name="speed"></param>移動速度。
/// <param name="modelRender"></param>モデルレンダー。
void CharacterBase::BaseMove(Vector3 position,Vector3 speed)
{
	//if (IsOnGround() == true) {
	//	//平面の移動速度をリセット。
	//	speed.x = 0.0f;
	//	speed.z = 0.0f;

	//	//重力をなくす。
	//	speed.y = 0.0f;
	//}
	//else {
	//	speed.y -= GRAVITY_AMOUNT;
	//}
}


Quaternion CharacterBase::BaseRotation(Quaternion rotation,Vector3 speed)
{
	Vector3 oldSpeed;
	Vector3 speedDiff;

	speedDiff = oldSpeed - speed;
	if (fabsf(speedDiff.x) >= 0.01f || fabsf(speedDiff.z) >= 0.01f) {
		rotation.SetRotationYFromDirectionXZ(speed);
		oldSpeed = speed;
	}

	return rotation;
}