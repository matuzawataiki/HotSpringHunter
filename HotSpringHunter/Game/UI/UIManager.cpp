#include "stdafx.h"
#include "UIManager.h"
#include "EnemyDetectionUI.h"

UIManager::UIManager()
{
	m_enemyInformationList.clear();
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
	// 警告シグナルの破棄
	DeleteTriangle();
	// 警告シグナルの生成と更新
	UpdateTriangle();
}

void UIManager::Render(RenderContext& rc)
{
}

void UIManager::UpdateTriangle()
{
	// 生成の処理
	for (EnemyInformation& information : m_enemyInformationList) {
		if (!information.m_isCreated) {
			// UIの生成
			information.m_ui = NewGO<EnemyDetectionUI>(1);
			information.m_isCreated = true;
		}
		information.m_ui->SetTargetPosition(information.m_position);
		information.m_ui->SetBasePosition(m_playerPosition);
		information.m_isUpdate = false;
	}
}

void UIManager::DeleteTriangle()
{
	// 削除の処理
	for (auto it = m_enemyInformationList.begin(); it != m_enemyInformationList.end();) {
		if (!it->m_isUpdate) {
			if (it->m_ui) {
				DeleteGO(it->m_ui);
				it->m_ui = nullptr;
			}
			it = m_enemyInformationList.erase(it);
		}
		else {
			++it;
		}
	}
}