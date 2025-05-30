#pragma once

class Player;
class EnemySpawn;
class EnemyBase;

//ヘビのステートの種類
enum EnSnakeState {
	enSnakeIdle,		//待機
	enSnakeTrack,		//追従
	enSnakeAttack,		//近接攻撃
	enSnakeKnockBack,	//ノックバック
	enSnakeDeath,		//死亡
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
	void LoadAsset();
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

	//セッター
	//ヘビの座標を設定
	void SetSnakePos(const Vector3& pos) { m_snakePos = pos; };
	//ヘビの出現状態を設定
	void SetIsSpawn(const bool& isSpawn) { m_isSpawn = isSpawn; };

private:

	CollisionObject*		collisionObject		= nullptr;
	Player*					m_player			= nullptr;
	EnemySpawn*				m_enemySpawn		= nullptr;
	EnemyBase*				m_enemyBase			= nullptr;

	AnimationClip			m_animationClips[enSnakeAnimClip_Num];  //アニメーションクリップ
	CharacterController		m_snakeController;						//キャラクターコントローラー	
	Quaternion				m_snakeRot = Quaternion::Identity;		//回転
	ModelRender				m_snakeModel;							//モデルレンダー

	Vector3 m_snakePos			= Vector3::Zero;			//座標
	Vector3 m_snakeSpeed		= Vector3::Zero;			//速度
	Vector3 m_snakeDir			= Vector3::Zero;			//向き
	Vector3 m_toPlayer			= Vector3::Zero;			//プレイヤーへのベクトル

	float m_snakeHP				= 0.0f;			//敵のHP
	float m_ATKCoolTime			= 0.0f;			//近接攻撃：クールタイム

	int m_snakeState			= 0;			//ヘビのステート

	bool m_isCanStateChange		= true;			//ステートを変えてもよいか
	bool m_isSpawn				= true;			//敵が出現するか
	bool m_isAlive				= true;			//敵が生きているか
	bool m_isRemoveController	= false;		//キャラコンを削除したか
};