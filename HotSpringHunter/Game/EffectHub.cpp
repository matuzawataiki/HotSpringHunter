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
	EffectEngine::GetInstance()->ResistEffect(enImpact, u"Assets/effect/playerAttack.efk");
}

void EffectHub::Update()
{

}
