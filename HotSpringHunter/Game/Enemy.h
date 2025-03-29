#pragma once

class Player;

//class CollisionObject;

class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();
	void Update();
	void Rotation();
	void Judgement();
	void Tracking();
	void EnemyHP();
	void EnemyAnimation();
	void ManageState();
	void EnemyAttackCollision();

	void Render(RenderContext& rc);

	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Attack,
		enAnimationClip_Num,
	};

	AnimationClip m_animationClips[enAnimationClip_Num];  //アニメーションクリップ

	Player* m_player;
	CharacterController m_characterController;  //キャラクターコントローター
	//CollisionObject* collisionObject;

	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3 m_toPlayer;
	Vector3 m_forward;
	Quaternion m_rotation;

	FontRender m_fontRender;  //フォントレンダー
	ModelRender m_modelRender;  //モデルレンダー
	PhysicsStaticObject physicsStaticObject;  //当たり判定

	enum EnemyState
	{
		enEnemyIdle,
		enEnemyWalk,
		enEnemyAttack,
		enEnemyNum,
	};
	int m_enemyState = enEnemyNum;  //エネミーステートの状態を表す変数

	bool m_moveState = false; //左右移動
	int m_enemyHP = 100;  //敵のHP


};


