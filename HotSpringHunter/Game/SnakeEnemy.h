#pragma once

class Player;
class EnemySpawn;
class EnemyBase;
class SoundEffect;

//ヘビのステートの種類
enum EnSnakeState {
	enSnakeIdle,
	enSnakeTrack,
	enSnakeAttack,
	enSnakeKnockBack,
	enSnakeDeath,
};

//ヘビのアニメーションクリップ
enum EnSnakeAnimClip
{
	enSnakeAnimClip_Idle,
	enSnakeAnimClip_Walk,
	enSnakeAnimClip_Attack,
	enSnakeAnimClip_Hit,
	enSnakeAnimClip_Death,
	enSnakeAnimClip_Num,
};

class SnakeEnemy : public IGameObject
{
public:
	SnakeEnemy();
	~SnakeEnemy();

	bool Start()override;
	//アセット読み込み
	void LoadAssets();
	void Update()override;
	//ステート管理。
	void ManageState();
	//行動を実行
	void ExecuteAction();
	//プレイヤーを探す
	bool FindPlayer();
	//いろいろ更新。
	void VariousUpdate();
	//速度を適応。
	void ExecuteSpeed();
	void Render(RenderContext& rc)override;

	CollisionObject*	collisionObject		= nullptr;
	Player*				m_player			= nullptr;
	EnemySpawn*			m_enemySpawn		= nullptr;
	EnemyBase*			m_enemyBase			= nullptr;
	EnemyHPBar*			m_hpBar		= nullptr;
	SoundEffect*        m_soundEffect = nullptr;	//サウンドエフェクト 

	AnimationClip		m_animationClips[enSnakeAnimClip_Num];  //アニメーションクリップ
	CharacterController m_characterController;					//キャラクターコントローラー	
	ModelRender			m_modelRender;							//モデルレンダー

	Quaternion			m_rotation = Quaternion::Identity;		//回転

	Vector3 m_position        = Vector3::Zero;				//座標
	Vector3 m_moveSpeed       = Vector3::Zero;				//速度
	Vector3 m_enemyDir        = Vector3::Zero;				//向き
	Vector3 m_toPlayer        = Vector3::Zero;				//プレイヤーへのベクトル

	float  m_enemyHP = 100.0f;		//敵のHP
	float m_ATKCoolTime = 0.0f;		//近接攻撃のクールタイム

	int m_snakeState = 0;			//ヘビのステート

	bool m_isSpawn = true;			//敵が出現するか
	bool m_isAlive = true;			//敵が生きているか
	bool m_isFind = false;			//プレイヤーを捉えたか
	bool m_isCanStateChange = true;	//ステートを変えてもよいか
};