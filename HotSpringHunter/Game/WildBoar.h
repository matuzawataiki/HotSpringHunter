#pragma once

namespace Character {
	class Player;
}
class EnemySpawn;
class EnemyBase;
class SoundEffect;

//イノシシのステート
enum EnWildBoarState
{
	enWildBoarIdle,			// 待機状態
	enWildBoarTrack,		// プレイヤーを追跡状態
	enWildBoarRun,			// 走る状態
	enWildBoarCharge,		// 突進状態
	enWildBoarAccum,		// 突進チャージ状態
	enWildBoarAttack,		// 攻撃状態
	enWildBoarKnockBack,	// ノックバック状態
	enWildBoarDeath			// 死亡状態
};

//イノシシのアニメーションクリップ
enum EnWildBoarAnimClip
{
	enWildBoarAnimClip_Idle,
	enWildBoarAnimClip_Walk,
	enWildBoarAnimClip_Run,
	enWildBoarAnimClip_Charge,
	enWildBoarAnimClip_Attack,
	enWildBoarAnimClip_Hit,
	enWildBoarAnimcClip_Death,
	enWildBoarAnimClip_Num,
};

class WildBoar : public IGameObject
{
public:
	WildBoar();
	~WildBoar();

	bool Start()override;
	//アセット読み込み
	void LoadAssets();
	void Update()override;
	//突進チャージ
	void Accumulate();
	//突進攻撃
	void Charge();
	//idle状態に戻す条件
	bool CanIdleState();
	//ステート
	void ManageState();
	//プレイヤーを探す
	bool FindPlayer();
	//行動実行
	void ExecuteAction();
	//いろいろ更新
	void VariousUpdate();
	//速度を適応
	void ExecuteSpeed();
	//突進コリジョン
	void ChargeCollision();
	//警告表示
	void ChargeCaveat();
	//モデル
	void Render(RenderContext& rc)override;

	//セッター
	//位置を設定
	inline void SetWildBoarPos(const Vector3& pos) { m_wildBoarPos = pos; };
	//向きを設定
	inline void SetWildBoarDir(const Vector3& dir) { m_wildBoarDir = dir; };
	//回転を設定
	inline void SetWildBoarRot(const Quaternion& rot) { m_wildBoarRot = rot; };
	//スポーン状態を設定
	inline void SetWildBoarIsSpawn(const bool isSpawn) { m_isSpawn = isSpawn; };
	//キャラコンの位置を設定
	inline void SetWildBoarCharaConPos(const Vector3& pos) { m_wildBoarController.SetPosition(pos); };

	//ゲッター
	//位置を取得
	inline Vector3 GetWildBoarPos() const { return m_wildBoarPos; };
	//スポーン状態を取得
	inline bool GetIsWildBoarIsSpawn() const { return m_isSpawn; };

private:

	CollisionObject*		collisionObject		= nullptr;
	Character::Player*		m_player			= nullptr;
	EnemySpawn*				m_enemySpawn		= nullptr;
	EnemyBase*				m_enemyBase			= nullptr;
	SoundEffect*			m_soundEffect		= nullptr; //サウンドエフェクト
	SoundSource*			m_chargeSound		= nullptr; //突進サウンド
	SoundSource*			m_runSound			= nullptr;    //突進攻撃サウンド

	AnimationClip			m_animationClips[enWildBoarAnimClip_Num];  //アニメーションクリップ
	CharacterController		m_wildBoarController;					   //キャラクターコントローラー
	Quaternion				m_wildBoarRot	= Quaternion::Identity;    //回転
	ModelRender				m_wildBoarModel;                           //モデルレンダー
	ModelRender				m_chargeCaveat;			                   //突進警告モデルレンダー

	Vector3 m_wildBoarPos		= Vector3::Zero;		//座標
	Vector3 m_wildBoarSpeed		= Vector3::Zero;		//速度
	Vector3 m_wildBoarDir		= Vector3::Zero;		//向き
	Vector3 m_toPlayer			= Vector3::Zero;		//ベクトル
	Vector3 m_toCharge			= Vector3::Zero;		//突進ベクトルの目的地
	Vector3 m_chargeVec			= Vector3::Zero;		//突進ベクトル
	Vector3 m_chargeOldPos		= Vector3::Zero;		//突進前の位置

	float m_wildBoarHP			= 0.0f;				//イノシシのHP
	float m_ATKCoolTime			= 0.0f;				//攻撃のクールタイム
	float m_chargeTime			= 0.0f;				//突進のチャージ時間
	float m_idleTime			= 0.0f;				//待機時間
	float m_chargeCoolTime		= 0.0f;				//突進クールタイム
	float m_elapsedTime			= 0.0f;				//死亡経過時間

	int m_wildBoarState			= 0;				//イノシシの行動状態

	bool m_isSpawn				= false;			//敵が出現するか
	bool m_isFind				= false;			//プレイヤーを捉えたか
	bool m_isCanStateChange		= true;				//ステートを変えてもよいか
	bool m_isHitCollision		= false;			//コリジョン当たったか
	bool m_isRemoveController	= false;			//キャラコンを消したか
	bool m_isChargeSoundPlay	= true;				//突進中か
};