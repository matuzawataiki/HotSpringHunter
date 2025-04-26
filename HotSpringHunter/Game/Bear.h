#pragma once
class Player;
class Bear:public IGameObject
{
public:
	Bear();
	~Bear();

	void Rotation();				//回転。
	void SnakeAttack();
	void Tracking();
	void EnemyAnimation();
	void MoveStop();
	void EnemyDeath();
	void Hit(float takeDamage);
	void ManageState();
	void EnemyAttackCollision();
	void KnockBack();
	void Update();


	void Render(RenderContext& rc);

	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Attack,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Num,
	};

	AnimationClip m_animationClips[enAnimationClip_Num];  //アニメーションクリップ

	CharacterController m_characterController;  //キャラクターコントローター
	CollisionObject* collisionObject = nullptr;

	Player* m_player = nullptr;

	Vector3 m_position = Vector3::Zero;		//座標
	Vector3 m_moveSpeed = Vector3::Zero;		//敵の速度
	Vector3 m_enemyDirection = Vector3::Zero;		//コリジョン
	Vector3 m_toPlayer = Vector3::Zero;		//ベクトル
	Quaternion m_rotation = Quaternion::Identity;		//クォータニオン

	ModelRender m_modelRender;  //モデルレンダー
	PhysicsStaticObject physicsStaticObject;  //当たり判定

	///


	//アニメーションステート
	enum AnimationState
	{
		enIdle,
		enWalk,
		enAttack,
		enHit,
		enDeath,
		enNum,
	};
	int m_animationState = enNum;  //エネミーステートの状態を表す変数

	Vector3 toPlayerDir;		//プレイヤーに向かって伸びるベクトル

	bool m_isSpawn = false;	//敵が出現するか
	bool m_isAlive = true;    //敵が生きているか
	bool m_enemyATK = false;    //敵が攻撃したか

	float  m_enemyHP = 50.0f;		//敵のHP
	bool m_moveStop = false;    //攻撃時に止まる

	int m_enemyDeathCuont = 0;  //敵が何匹死んだ

	float posX = 0.0f;
	float posY = 0.0f;

	bool enemyDead = false; //死んだ処理を一回だけ
};