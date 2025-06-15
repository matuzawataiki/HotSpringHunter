#pragma once
enum EnEffectVar {
	enImpact,
	enEnemyHit,
	enPlayerAttack,
	enCharge01,
	enCharge02,
	enCharge03,
};

class EffectHub:public IGameObject
{
public :
	EffectHub();
	~EffectHub();
	bool Start()override;
	//エフェクトをロード
	void LoadEffect();
	void Update()override;

private:

};

