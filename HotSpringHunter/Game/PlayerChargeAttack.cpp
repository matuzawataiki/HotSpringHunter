#include "stdafx.h"
#include "PlayerChargeAttack.h"
#include "Player.h"
#include "SnakeEnemy.h"
#include "collision/CollisionObject.h"

namespace {
	const float CHARGE_DECREASE = 0.4f;			//チャージ減少量。
	const float COLLISION_SIZE = 3.0f;
}

PlayerChargeAttack::PlayerChargeAttack()
{

}

PlayerChargeAttack::~PlayerChargeAttack()
{

}

bool PlayerChargeAttack::Start()
{
	m_player = FindGO<Player>("player");

	return true;
}

void PlayerChargeAttack::Update()
{
	StickCharge();
	ChargeAttack();
	DisplayCharge();
}

void PlayerChargeAttack::StickCharge()
{
	Vector3 RStick = Vector3::Zero;				//Rスティック入力量。	
	float movePower = 0.0f;						//パワー（入力変動量）。

	//Rスティックの入力があったら。
	if ((fabsf(g_pad[0]->GetRStickXF()) >= 0.001f) || (fabsf(g_pad[0]->GetRStickYF()) >= 0.001f)) {
		//アニメーションステート切り替え。
		//m_player->m_animationState = m_player->enCharging;

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

	if (m_charge >= 100.0f) {
		m_charge = 100.0f;
	}

	//スティック入力量を更新。
	m_RStickOld = RStick;

}

void PlayerChargeAttack::ChargeAttack()
{
	//チャージが20以上でスティックの入力がなかったら。
	if ((fabsf(g_pad[0]->GetRStickXF()) <= 0.001f) || (fabsf(g_pad[0]->GetRStickYF()) <= 0.001f)) {
		if (m_charge >= 20.0f) {
			//playerのアニメーションステート切り替え。
			m_player->m_animationState = m_player->enChargeAttack;
			SnakeEnemy* snakeEnemy = FindGO<SnakeEnemy>("enemy");
			/*if (snakeEnemy == nullptr) {
				return;
			}*/
			MakeCollision();
			//if (m_collision->IsHit(snakeEnemy->m_characterController)) {
			//	//snakeEnemy->Hit(m_charge * 2.0f);
			//}
			DeleteGO(m_collision);
		}
	}
}

void PlayerChargeAttack::MakeCollision()
{
	//コリジョンオブジェクトを作成
	m_collision = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_player->GetPlayerPos();
	m_collisionSize = COLLISION_SIZE * m_charge + 100.0f;
	//球状のコリジョンを作成
	m_collision->CreateSphere(collisionPosition,
		Quaternion::Identity,
		m_collisionSize);
	//名前をつける
	m_collision->SetName("chargeAttack");
	//チャージリセット。
	m_charge = 0.0f;
}

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

