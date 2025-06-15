#pragma once
enum EnEffectVar {
	enImpact,
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

