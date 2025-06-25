#pragma once
#include"SoundEffect.h"

//サウンドエフェクト
enum Sound
{
	//プレイヤーのSE
	enPlayerAttackSE,
	enPlayerHitSE,
	enPlayerDamageSE,
	enPlayerGuardSE,
	enPlayerCharge1SE,
	enPlayerCharge2SE,
	enPlayerCharge3SE,
	enPlayerDeathSE,  

	//敵のSE
	//ヘビ
	enSnakeAttackSE,
	enSnakeHitSE,
	enSnakeDeathSE,
	//イノシシ
	enWildBoarAttackSE, 
	enWildBoarChargeSE,
	emWildBoarRunSE,
	enwildBoarCahrgeAttackSE,
	enWildBoarHitSE,
	enWildBoarDeathSE,
	//クマ
	enBearRoarSE,
	enBearAttackSE,
	enBearStoneAttackSE,
	enBearHitSE,
	enBearDeathSE,

	//ゲーム内BGM
	enTitleBGM,
	enNomalBGM,
	enBossBGM,
	enGameClearBGM,
	enGameOverBGM,

	enSoundNumSE
};

class SoundEffect : public IGameObject
{
public:
	SoundEffect();
	~SoundEffect();
	
	bool Start() override;
	void Update() override;

	/// <summary>
	/// 指定したサウンドを再生します。
	/// </summary>
	/// <param name="sound">再生するサウンドの識別子。</param>
	/// <param name="loop">サウンドをループ再生する場合は true、1 回だけ再生する場合は false。</param>
	SoundSource* Play(int sound, bool loop);
	void Play(int bgm);

	/// <summary>
	/// 指定したインデックスの処理を停止します。
	/// </summary>
	/// <param name="index">停止する対象のインデックス。</param>
	void Stop(SoundSource*soundSorce);

	void StopBGM();

private:
	std::vector<SoundSource*> m_gameSEList;
	SoundSource* m_gameBGM = nullptr;
	
};

