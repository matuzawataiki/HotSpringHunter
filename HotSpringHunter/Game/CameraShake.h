#pragma once

enum ShakeStrength
{
	Weak,		//弱い
	Medium,		//中くらい
	Storong,	//強い
};

struct CameraShakeParameter
{
	bool isActive = false;       //揺れているかどうか
	float durationTime =  0.0f; //揺れている時間
	float elapsedTime = 0.0f;   //経過時間

	ShakeStrength strength; //揺れの強さ
	float intensity = 0.0f; //揺れの強さの値
	float frequency = 5.0f; //揺れの周波数
	float seed = 0.0f;      //揺れの方向をランダムにするためのシード値

};

class CameraShake
{
public:
	CameraShake();
	~CameraShake();

public:
	void Update(const float deltaTime, Vector3& eyePosition, Vector3& targetPosition);
	void StartShake(const ShakeStrength strength, const float durationTime, const float intensity, const float frequency = 5.0f);

private:
	void ApplyShake(const float deltaTime, Vector3& eyePosition, Vector3& targetPosition);

	//揺れの強さに応じた倍率を取得する関数(Multiplier = 乗算する)
	float GetStrengthMultiplier(const ShakeStrength strength) const
	{
		switch (strength)
		{
			case Weak:    return 0.5f;
			case Medium:  return 1.0f;
			case Storong: return 1.5f;
		}
		return 0.5f;
	}

	//簡易的な1D次元パーリンノイズ関数
	float Perlin1D(float x)
	{
		//0~1のsinカーブノイズ
		return sinf(x * 2.0f * 3.14159f) * 0.5f;
	}

private:
	//変数
	CameraShakeParameter parameter;
};