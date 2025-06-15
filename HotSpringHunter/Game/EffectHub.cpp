#include "stdafx.h"
#include "EffectHub.h"

EffectHub::EffectHub()
{
}

EffectHub::~EffectHub()
{

}

bool EffectHub::Start()
{
	LoadEffect();

	return true;
}

/// <summary>
/// エフェクトをロード
/// </summary>
void EffectHub::LoadEffect()
{
	EffectEngine::GetInstance()->ResistEffect(enImpact, u"Assets/effect/impact.efk");
	EffectEngine::GetInstance()->ResistEffect(enEnemyHit, u"Assets/effect/enemyHit.efk");
	EffectEngine::GetInstance()->ResistEffect(enPlayerAttack, u"Assets/effect/playerAttack.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge01, u"Assets/effect/charge01.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge02, u"Assets/effect/charge02.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge03, u"Assets/effect/charge03.efk");
}

void EffectHub::Update()
{

}
