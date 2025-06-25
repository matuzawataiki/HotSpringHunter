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
	EffectEngine::GetInstance()->ResistEffect(enImpact, u"Assets/effect/impact/impact.efk");
	EffectEngine::GetInstance()->ResistEffect(enEnemyHit, u"Assets/effect/enemyHit/enemyHit.efk");
	EffectEngine::GetInstance()->ResistEffect(enRoar, u"Assets/effect/roar/roar.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge, u"Assets/effect/chargeAttack/chargeAttack.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge01, u"Assets/effect/charge/charge01.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge02, u"Assets/effect/charge/charge02.efk");
	EffectEngine::GetInstance()->ResistEffect(enCharge03, u"Assets/effect/charge/charge03.efk");
}

void EffectHub::Update()
{

}
