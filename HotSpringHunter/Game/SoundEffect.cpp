#include "stdafx.h"
#include "SoundEffect.h"
#include "sound/SoundEngine.h"


SoundEffect::SoundEffect() 
{
	g_soundEngine->ResistWaveFileBank(enPlayerAttackSE, "Assets/Sound/player/player_attack.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerHitSE, "Assets/Sound/player/player_hit.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerDamageSE, "Assets/Sound/player/player_damage.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerGuardSE, "Assets/Sound/player/player_guard.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerCharge1SE, "Assets/Sound/player/charge1.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerCharge2SE, "Assets/Sound/player/charge2.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerCharge3SE, "Assets/Sound/player/charge3.wav");
	g_soundEngine->ResistWaveFileBank(enPlayerDeathSE, "Assets/Sound/player/player_death.wav");

	//ヘビ
	g_soundEngine->ResistWaveFileBank(enSnakeAttackSE, "Assets/Sound/enemy/snake_attack.wav");
	g_soundEngine->ResistWaveFileBank(enSnakeHitSE, "Assets/Sound/enemy/enemy_hit.wav");
	g_soundEngine->ResistWaveFileBank(enSnakeDeathSE, "Assets/Sound/enemy/enemy_death.wav");

	//イノシシ
	g_soundEngine->ResistWaveFileBank(enWildBoarAttackSE, "Assets/Sound/enemy/wildboar_attack.wav");
	g_soundEngine->ResistWaveFileBank(enWildBoarChargeSE, "Assets/Sound/enemy/wildboar_charge.wav");
	g_soundEngine->ResistWaveFileBank(emWildBoarRunSE, "Assets/Sound/enemy/wildboar_run.wav");
	g_soundEngine->ResistWaveFileBank(enwildBoarCahrgeAttackSE, "Assets/Sound/enemy/wildboar_charge_attack.wav");
	g_soundEngine->ResistWaveFileBank(enWildBoarHitSE, "Assets/Sound/enemy/enemy_hit.wav");
	g_soundEngine->ResistWaveFileBank(enWildBoarDeathSE, "Assets/Sound/enemy/enemy_death.wav");

	//クマ
	g_soundEngine->ResistWaveFileBank(enBearRoarSE, "Assets/Sound/enemy/bear_roar.wav");
	g_soundEngine->ResistWaveFileBank(enBearAttackSE, "Assets/Sound/enemy/bear_attack.wav");
	g_soundEngine->ResistWaveFileBank(enBearStoneAttackSE, "Assets/Sound/enemy/bear_stonethrow.wav");
	g_soundEngine->ResistWaveFileBank(enBearHitSE, "Assets/Sound/enemy/enemy_hit.wav");
	g_soundEngine->ResistWaveFileBank(enBearDeathSE, "Assets/Sound/enemy/bear_death.wav");

	//BGM
	g_soundEngine->ResistWaveFileBank(enTitleBGM, "Assets/Sound/bgm/titleBGM.wav");
	g_soundEngine->ResistWaveFileBank(enNomalBGM, "Assets/Sound/bgm/nomalBGM.wav");
	g_soundEngine->ResistWaveFileBank(enBossBGM, "Assets/Sound/bgm/bossBGM.wav");
	g_soundEngine->ResistWaveFileBank(enGameClearBGM, "Assets/Sound/bgm/clearBGM.wav");
	g_soundEngine->ResistWaveFileBank(enGameOverBGM, "Assets/Sound/bgm/overBGM.wav");


	m_gameSEList.clear();
}

SoundEffect::~SoundEffect() 
{

}

bool SoundEffect::Start()
{
	return true;	
}

void SoundEffect::Update()
{
	for (auto it = m_gameSEList.begin(); it != m_gameSEList.end(); )
	{
		if (!(*it)->IsPlaying())
		{
			DeleteGO(*it);                  // 実際の削除処理
			it = m_gameSEList.erase(it);   // erase は次のイテレータを返す
		}
		else
		{
			++it;
		}
	}
}

SoundSource* SoundEffect::Play(int sound , bool loop)
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(sound);
	se->Play(loop);

	m_gameSEList.push_back(se);

	return se;
}

void SoundEffect::Play(int bgm)
{
	if (m_gameBGM != nullptr)
	{
		DeleteGO(m_gameBGM);
	}
	m_gameBGM = NewGO<SoundSource>(0);
	m_gameBGM->Init(bgm);
	m_gameBGM->Play(true); // BGMはループ再生
}

void SoundEffect::Stop(SoundSource* soundSource)
{
	if (soundSource != nullptr)
	{
		soundSource->Stop();
	}
}

void SoundEffect::StopBGM() 
{
	m_gameBGM->Stop();
}