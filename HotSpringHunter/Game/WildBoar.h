#pragma once

class Player;
class EnemySpawn;
class EnemyBase;

//イノシシのステート
enum EnWildBoarState
{
	enWildBoarIdle,
};

//イノシシのアニメーションクリップ
enum EnWildBoarAnimClip
{
	enWildBoarAnimClip_Idle,
	enWildBoarAnimClip_Num,
};

class WildBoar : public IGameObject
{
public:
	WildBoar();
	~WildBoar();

	bool Start()override;
	void Update()override;
	void ManageState();
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

	bool m_isSpawn = true;	//敵が出現するか
	bool m_isAlive = true;    //敵が生きているか
	bool m_isFind = false;			//プレイヤーを捉えたか
	bool m_isCanStateChange = true;	//ステートを変えてもよいか
};