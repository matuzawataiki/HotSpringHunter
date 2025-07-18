#include "stdafx.h"
#include "CameraShake.h"
#include "GameCamera.h"

CameraShake::CameraShake()
{
}

CameraShake::~CameraShake()
{

}

void CameraShake::Update(const float deltaTime, Vector3& eyePosition, Vector3& targetPosition)
{
	// 更新。画ブレを適用する
	ApplyShake(deltaTime, eyePosition, targetPosition);
}

void CameraShake::StartShake(const ShakeStrength strength, const float durationTime, const float intensity, const float frequency)
{
	parameter.isActive = true;
	parameter.durationTime = durationTime;
	parameter.elapsedTime = 0.0f;
	parameter.strength = strength;
	parameter.intensity = intensity;
	parameter.frequency = frequency;

	//ランダムなシードを設定(複数ノイズを生成)
	//0~9999の範囲で乱数を生成
	parameter.seed = rand() % 10000;
}

//ノイズを加算
void CameraShake::ApplyShake(const float deltaTime, Vector3& eyePosition, Vector3& targetPosition)
{
	//カメラシェイクが有効な場合のみ処理を行う
	if (!parameter.isActive) return;

	parameter.elapsedTime += deltaTime;
	if (parameter.elapsedTime >= parameter.durationTime)
	{
		//指定時間が経過したらシェイクを終了
		parameter.isActive = false;
		return;
	}

	//経過時間* 周波数を掛けて時間を計算
	float t = parameter.elapsedTime * parameter.frequency;

	//揺れる強度を求める
	float strengthMultiplier = GetStrengthMultiplier(parameter.strength);
	//shakeAmount = シェイク量
	float shakeAmount = parameter.intensity * strengthMultiplier;

	//揺れの強さ
	float shakeX = Perlin1D(t + parameter.seed) * shakeAmount;
	float shakeY = Perlin1D(t + 100.0f + parameter.seed) * shakeAmount; // 100を足して別のノイズを生成
	float shakeZ = Perlin1D(t + 200.0f + parameter.seed) * shakeAmount; // 200を足して別のノイズを生成

	//カメラの位置に揺れを加える
	//eye = 注視点
	eyePosition.x += shakeX;
	eyePosition.y += shakeY;
	eyePosition.z += shakeZ;

	//ターゲットの位置に揺れを加える
	// target = 注視点のターゲット
	targetPosition.x += shakeX;
	targetPosition.y += shakeY;
	targetPosition.z += shakeZ;
}