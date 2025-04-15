#include "stdafx.h"
#include "PlayerChargeAttack.h"
#include "Player.h"

namespace {
	const float CHARGE_DECREASE = 0.1f;			//チャージ減少量。
}

PlayerChargeAttack::PlayerChargeAttack()
{

}

PlayerChargeAttack::~PlayerChargeAttack()
{

}

bool PlayerChargeAttack::Start()
{
	return true;
}

void PlayerChargeAttack::Update()
{
	StickCharge();
	DisplayCharge();
}

void PlayerChargeAttack::StickCharge()
{
	Vector3 RStick = Vector3::Zero;				//Rスティック入力量。	
	float movePower = 0.0f;						//パワー（入力変動量）。

	//Rスティックの入力があったら。
	if ((fabsf(g_pad[0]->GetRStickXF()) >= 0.001f) || (fabsf(g_pad[0]->GetRStickYF()) >= 0.001f)) {
		//Rスティックの入力量をとる。		
		RStick.x = g_pad[0]->GetRStickXF();
		RStick.y = g_pad[0]->GetRStickYF();
		//チャージ量にたすパワーを計算する（スティックの変更後、変更前の内積）。
		movePower = Dot(RStick, m_RStickOld);
		//スティックが動いていないなら、パワーを0にする。
		if ((RStick.x == m_RStickOld.x) && (RStick.y == m_RStickOld.y)) {
			movePower = 0.0f;
		}
		//パワーの値がマイナスならプラスにする。
		if (movePower < 0.0f) {
			movePower *= -1.0f;
		}
		//パワーをチャージに足す。
		m_charge += movePower;
	}
	//パワーリセット。
	movePower = 0.0f;

	//チャージを減少させる。
	m_charge -= CHARGE_DECREASE;

	//チャージを0以下にさせない。
	if (m_charge <= 0.0f) {
		m_charge = 0.0f;
	}

	//スティック入力量を更新。
	m_RStickOld = RStick;
}

/// <summary>
/// チャージ量表示（仮）
/// </summary>
void PlayerChargeAttack::DisplayCharge()
{
	m_chargeRender.SetScale(1.2);
	m_chargeRender.SetPosition({ 425.0f,475.0f,0.0f });
	m_chargeRender.SetColor(g_vec4Black);

	swprintf_s(m_chargeText, 100, L"チャージ %.1f", float(m_charge));
	m_chargeRender.SetText(m_chargeText);
}


void PlayerChargeAttack::Render(RenderContext& rc) 
{
	m_chargeRender.Draw(rc);
}

