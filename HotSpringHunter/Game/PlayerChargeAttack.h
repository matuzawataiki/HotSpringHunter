#pragma once
class Player;
class PlayerChargeAttack:public IGameObject
{
public:
	PlayerChargeAttack();
	~PlayerChargeAttack();
	bool Start()override;
	void Update()override;
	void StickCharge();				//チャージ蓄積。
	void DisplayCharge();			//チャージ量表示（仮）。
	void Render(RenderContext& rc)override;

	Player* m_player = nullptr;

	Vector3 m_RStickOld = Vector3::Zero;				//Rスティックの入力量（変更前）。
	float m_charge = 0.0f;								//チャージ量。

	//チャージ量表示（仮）
	FontRender m_chargeRender;
	wchar_t m_chargeText[100];
};

