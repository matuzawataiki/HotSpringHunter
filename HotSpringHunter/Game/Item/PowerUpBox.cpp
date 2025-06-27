#include "stdafx.h"
#include "PowerUpBox.h"
#include "Player.h"
#include "BackGround/StageManager.h"
#include "ItemExplanation.h"

namespace {
	float XM_2PI = 6.283185307f;
}

PowerUpBox::PowerUpBox()
{
}

PowerUpBox::~PowerUpBox()
{
}

bool PowerUpBox::Start()
{
	const char* filePath = "";
	StageManager* stageManager = FindGO<StageManager>("stageManager");

	switch (m_num)
	{
	case PowerUpBox::enSlash:	//遠距離攻撃
		filePath = "Assets/modelData/powerUpBox/PowerUpBox1.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage1).fence[1]->GetPosition();
		m_position.x -= 200.0f;
		break;

	case PowerUpBox::enSlash1:	//チャージで弾数増加
		filePath = "Assets/modelData/powerUpBox/PowerUpBox1_1.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage2).fence[1]->GetPosition();
		m_position.x -= 200.0f;
		break;

	case PowerUpBox::enSlash2:	//サイズ強化
		filePath = "Assets/modelData/powerUpBox/PowerUpBox1_2.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage2).fence[1]->GetPosition();
		m_position.x += 200.0f;
		break;

	case PowerUpBox::enRangeAttack:	//範囲攻撃
		filePath = "Assets/modelData/powerUpBox/PowerUpBox2.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage1).fence[1]->GetPosition();
		m_position.x += 200.0f;
		break;

	case PowerUpBox::enRangeAttack2://範囲強化
		filePath = "Assets/modelData/powerUpBox/PowerUpBox2_2.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage2).fence[1]->GetPosition();
		m_position.x -= 200.0f;
		break;

	case PowerUpBox::enRangeAttack3://チャージ中範囲に持続ダメージ
		filePath = "Assets/modelData/powerUpBox/PowerUpBox2_3.tkm";
		m_position = stageManager->GetStageObject(StageManager::enBattleStage2).fence[1]->GetPosition();
		m_position.x += 200.0f;
		break;

	case PowerUpBox::enNum:
		filePath = "";
		break;

	default:
		break;
	}

	m_position.y += 50.0f;

	m_boxModel.SetPosition(m_position);

	m_boxModel.Init(filePath);

	return true;
}

void PowerUpBox::Update()
{
	m_plaeyr = FindGO<Character::Player>("player");
	Vector3 length = m_position - m_plaeyr->GetPlayerPos();
	length.y = 0.0f;
	if (length.Length() <= 100.0f) {
		DeleteGO(this);
		m_plaeyr->GetPowerUp(m_num);

		//アイテムの説明を表示
		m_itemExplanation = FindGO<ItemExplanation>("itemExplanation");
		m_itemExplanation->DrawImage(m_num);
	}

	m_time += g_gameTime->GetFrameDeltaTime();
	float amplitude = 1.6f; // 上下の振幅（最大移動量）
	float speed = 0.5f;     // 上下の速さ（周期）

	float yOffset = amplitude * sinf(m_time * speed * XM_2PI);
	Vector3 position = m_position;
	position.y *= yOffset;

	m_boxModel.SetPosition(position);
	m_boxModel.Update();

}

void PowerUpBox::Render(RenderContext& rc)
{
	m_boxModel.Draw(rc);
}
