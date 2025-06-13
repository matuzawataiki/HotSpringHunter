#include "stdafx.h"
#include "SmokeEmitter.h"

#include "Smoke.h"

namespace
{
	const Vector2 SMOKE_POSITION[] = {
		Vector2(-800.0f,-600.0f),	//左下
		Vector2(800.0f,-600.0f),	//右下
		Vector2(-200.0f,-600.0f),	//真ん中
		Vector2(200.0f,-600.0f),	//真ん中
		Vector2(-400.0f,-600.0f),	//左真ん中
		Vector2(400.0f,-600.0f),	//右真ん中
		Vector2(-600.0f,-600.0f),	//左真ん中
		Vector2(600.0f,-600.0f),	//右真ん中
	};

	const float MAKE_SMOKE_TIME = 0.2f;	//雲が生成される時間/毎
}

SmokeEmitter::SmokeEmitter()
{
}

SmokeEmitter::~SmokeEmitter()
{
	//湯気の削除
	for (Smoke* smoke : m_smokeList)
	{
		DeleteGO(smoke);
	}
	m_smokeList.clear();
}

bool SmokeEmitter::Start()
{
	return true;
}

void SmokeEmitter::Update()
{
	m_mekeSmokeElapsedTime += g_gameTime->GetFrameDeltaTime();

	if (m_isActiveEmitter == true)
	{
		if (m_mekeSmokeElapsedTime >= MAKE_SMOKE_TIME)
		{
			m_mekeSmokeElapsedTime = 0.0f;

			const int smokeType = rand() % enSmokeTypeMax;

			//湯気の種類が3種類あるからそれらをランダムに出す
			Smoke* smoke = nullptr;
			smoke = NewGO<Smoke>(0);
			smoke->SetSmokeType(smokeType);
			smoke->SetSmokePos(SMOKE_POSITION[m_positionIndex]);
			m_smokeList.push_back(smoke);
			m_positionIndex++;

			//5つの配列posから出たら配列をまた0に戻す
			if (m_positionIndex >= ARRAYSIZE(SMOKE_POSITION))
			{
				m_positionIndex = 0;
			}
		}
	}

	//上にのぼっていく
	for (Smoke* smoke : m_smokeList)
	{
		Vector2 smokePos = smoke->GetSmokePos();
		smokePos.y += 8.0f;
		smoke->SetSmokePos(smokePos);
	}
}

void SmokeEmitter::Render(RenderContext& rc)
{

}