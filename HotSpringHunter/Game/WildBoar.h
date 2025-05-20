#pragma once

class Player;
class EnemySpawn;
class EnemyBase;

//イノシシのステート
enum EnWildBoarState
{
	enWildBoarIdle,
	enWildBoarTrack,
	enWildBoarRun,
	enWildBoarCharge,
	enWildBoarAccum,
	enWildBoarAttack,
	enWildBoarKnockBack,
	enWildBoarDeath
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

	//モデル
	void Render(RenderContext& rc)override;

	CollisionObject* collisionObject = nullptr;
	Player*          m_player = nullptr;
	EnemySpawn*      m_enemySpawn = nullptr;
	EnemyBase*       m_enemyBase = nullptr;

	AnimationClip m_animationClips[enWildBoarAnimClip_Num];  //アニメーションクリップ
	CharacterController m_characterController;               //キャラクターコントローラー
	Quaternion m_rotation = Quaternion::Identity;            //回転
	ModelRender m_wildBoarModel;                                  //モデルレンダー

	Vector3 m_position = Vector3::Zero; //座標
	Vector3 m_moveSpeed = Vector3::Zero; //速度
	Vector3 m_enemyDir = Vector3::Zero; //向き
	Vector3 m_toPlayer = Vector3::Zero; //ベクトル

	float m_wildBoarHP = 100.0f; //イノシシのHP
	float m_ATKCoolTime = 0.0f;			//近接攻撃のクールタイム

	//行動状態
	int m_wildBoarState = 0;

	bool m_isSpawn = true;	//敵が出現するか
	bool m_isAlive = true;    //敵が生きているか
	bool m_isFind = false;			//プレイヤーを捉えたか
	bool m_isCanStateChange = true;	//ステートを変えてもよいか
};