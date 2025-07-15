#include "stdafx.h"
#include "UIManager.h"
#include "EnemyDetectionUI.h"

UIManager::UIManager()
{
	m_enemyInfomationList.clear();
}

UIManager::~UIManager()
{
}

bool UIManager::Start()
{
	return true;
}

void UIManager::Update()
{
	// 警告シグナルの生成と更新
	UpdateTriangle();
	// 警告シグナルの破棄
	DeleteTriangle();
}

void UIManager::Render(RenderContext& rc)
{
}

void UIManager::UpdateTriangle()
{
	// 生成の処理
	for (EnemyInfomation& infomation : m_enemyInfomationList) {
		if (!infomation.m_isCreated) {
			// UIの生成
			infomation.m_ui = NewGO<EnemyDetectionUI>(1);
			infomation.m_isCreated = true;
		}
		infomation.m_ui->SetTargetPosition(infomation.m_position);
		infomation.m_ui->SetBasePosition(m_playerPosition);
	}
}

void UIManager::DeleteTriangle()
{
	// 削除の処理
	for (auto it = m_enemyInfomationList.begin(); it != m_enemyInfomationList.end();) {
		if (it->m_requestDelete) {
			if (it->m_ui) {
				DeleteGO(it->m_ui);
				it->m_ui = nullptr;
			}
			it = m_enemyInfomationList.erase(it);
		}
		else {
			++it;
		}
	}
}