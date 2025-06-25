#include "stdafx.h"
#include "ItemExplanation.h"
#include "Player.h"

namespace {
	const float IMAGE_DISPLAY_TIME = 5.0f; // 画像の表示時間
}

ItemExplanation::ItemExplanation()
{
}

ItemExplanation::~ItemExplanation()
{
}

bool ItemExplanation::Start()
{
	m_itemImageBack.Init("Assets/modelData/image/explanationBack.DDS", 700.0f, 150.0f);
	m_itemImageBack.SetPosition(-650.0f, 300.0f);
	m_itemImageBack.Update();
	
	return true;
}

void ItemExplanation::Update()
{
	HideImage();
}

/// <summary>
/// 画像を描画します。
/// </summary>
void ItemExplanation::DrawImage()
{
	m_isDrawImage = true;

}

/// <summary>
/// テキストを設定します。
/// </summary>
void ItemExplanation::SetText(int base,int derivative)
{
	wchar_t text[256];

	if (base == 1) {
		if (derivative == 0) {
			swprintf_s(text, 256, L"攻撃をすると、波動が出る！！");
		}
		else if (derivative == 1) {
			swprintf_s(text, 256, L"チャージで波動が増える！！");
		}
		else if (derivative == 2) {
			swprintf_s(text, 256, L"波動が大きくなる！！");

		}
	}
	else if (derivative == 2) {
		if (derivative == 0) {
			swprintf_s(text, 256, L"チャージ攻撃が広範囲攻撃に強化！！");
		}
		else if (derivative == 0) {
			swprintf_s(text, 256, L"チャージ攻撃の範囲拡大！！");
		}
		else if (derivative == 0) {
			swprintf_s(text, 256, L"チャージ中に、近くの敵にダメージ！！");
		}
	}

	m_itemImageText.SetText(text);
	m_itemImageText.SetPosition(-950.0f, 320.0f);
	m_itemImageText.SetScale(0.8f);
	m_itemImageText.SetColor(g_vec4Black);

	m_isDrawImage = true;
}

/// <summary>
/// 画像を非表示にします。
/// </summary>
void ItemExplanation::HideImage()
{
	if(!m_isDrawImage) {
		return;
	}

	//画像の表示時間をカウント
	m_elapsedTime += g_gameTime->GetFrameDeltaTime();

	if (m_elapsedTime >= IMAGE_DISPLAY_TIME) {
		m_isDrawImage = false;
	}
}

void ItemExplanation::Render(RenderContext& rc)
{
	if (m_isDrawImage) {
		m_itemImageBack.Draw(rc);
		m_itemImageText.Draw(rc);
	}
}
