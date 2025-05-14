#pragma once
class Player;
class EnemySpawn;
class EnemyBase;

//クマの行動状態
enum EnBearState {
	enBearIdle,
	enBearTrack,
	enBearMeleeAttack,
	enBearKnockBack,
	enBearDeath,
};

//クマのアニメーションクリップ
enum EnBearAnimClip
{
	enBearAnimClip_Idle,
	enBearAnimClip_Run,
	enBearAnimClip_Attack,
	enBearAnimClip_Hit,
	enBearAnimClip_Death,
	enBearAnimClip_Num,
};

class Bear:public IGameObject
{
public:
	Bear();
	~Bear();
	bool Start()override;
	//アセットを読み込む
	void LoadAssets();
	void Update()override;
	//ステート管理
	void ManageState();
	//
	void DirUpdate();
	//行動実行
	void ExecuteAction();
	//いろいろ更新
	void VariousUpdate();
	//速度を適応
	void ExecuteSpeed();
	void Render(RenderContext& rc)override;

	Player*             m_player		= nullptr;
	EnemySpawn*         m_enemySpawn	= nullptr;
	EnemyBase*          m_enemyBase		= nullptr;

	Quaternion m_rotation = Quaternion::Identity;			//クォータニオン
	ModelRender m_modelRender;								//モデルレンダー
	AnimationClip m_animationClips[enBearAnimClip_Num];		//アニメーションクリップ
	CharacterController m_characterController;				//キャラクターコントローター

	Vector3 m_position       = Vector3::Zero;		//座標
	Vector3 m_moveSpeed      = Vector3::Zero;		//敵の速度
	Vector3 m_enemyDir       = Vector3::Zero;		//向き
	Vector3 m_toPlayer       = Vector3::Zero;		//ベクトル

	float m_enemyHP = 100.0f;			//敵のHP
	float m_ATKCoolTime = 0.0f;			//近接攻撃のクールタイム

	int m_bearState = 0;				//クマの行動状態

	bool m_isCanStateChange = true;		//ステート変更を受け付けているか
	bool m_isSpawn = true;				//敵が出現するか
	bool m_isAlive = true;				//敵が生きているか
	bool m_findFlag = false;			//プレイヤーを発見したか
};