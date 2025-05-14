#pragma once
class Towel;
class Bucket;
class IState;
class StateMachine;
class SnakeEnemy;

//現在アクティブなステート。
//順番気をつけようね。
enum EnPlayerActiveState{
	enPlayerIdle,
	enPlayerWalk,
	enPlayerWeakAttack,
	enPlayerChargeAttack,
	enPlayerGuard,
	enPlayerHit,
	enPlayerDeath,
};

//アニメーションクリップ。
enum EnPlayerAnimClip {
	enPlayerAnimClip_Idle,
	enPlayerAnimClip_Walk,
	enPlayerAnimClip_Run,
	enPlayerAnimClip_Jump,
	enPlayerAnimClip_GuardStart,
	enPlayerAnimClip_GuardEnd,
	enPlayerAnimClip_WeakAttack,
	enPlayerAnimClip_ChargeAttack,
	enPLayerAnimClip_Charging,
	enPlayerAnimClip_Hit,
	enPlayerAnimClip_Death,
	enPlayerAnimClip_Num,
};

class Player :public IGameObject
{
public:
	Player();
	~Player();
	//リスト削除。
	void DeleteList();
	bool Start()override;
	//リスト追加。
	void AddList();
	//アセットロード。
	void LoadAssets();	
	void Update()override;
	//基本的な挙動。
	void BasicBehavior();
	//向きを更新。
	void DirectionUpdate();
	//被弾。
	void Hit(float reduce);
	//state管理。
	void StateManage();
	//チャージ量表示（仮）。
	void DisplayCharge();
	void Render(RenderContext& rc)override;

	//player座標のゲッター。
	Vector3 GetPlayerPos() {
		return m_playerPos;
	}
	//playerの向きのゲッター。
	Vector3 GetPlayerDir() {
		return m_playerDir;
	}

	//攻撃力のゲッター。
	float GetAttackPower() {
		return m_attackPower;
	}
	//チャージのゲッター。
	float GetCharge() {
		return m_charge;
	}

	//チャージ量表示（仮）
	FontRender m_chargeRender;
	wchar_t m_chargeText[100];


	std::vector<IState*>m_stateList;
	int m_currentState = 0;						//現在のステート。
	int m_requestState = 0;						//変更したいステート。

	StateMachine* m_stateMachine = nullptr;
	Towel* m_towel = nullptr;
	Bucket* m_bucket = nullptr;

	SnakeEnemy* m_snakeEnemy = nullptr;

	CollisionObject* m_collision = nullptr;


	CharacterController m_playerCharaCon;					//キャラコン。
	ModelRender m_playerModel;								//描画。
	Vector3 m_playerPos = Vector3::Zero;				//座標。
	Vector3 m_playerSpeed = Vector3::Zero;					//移動スピード。
	Vector3 m_playerDir = Vector3::Zero;				//向き。
	Quaternion m_playerRot = Quaternion::Identity;		//回転。

	float m_playerHP = 10000.0f;								//player体力。

	float m_dashState = 1.0f;								//ダッシュ：走り状態の移動管理。


	float m_guardState = 1.0f;								//ガード：ガード状態の移動管理。
	bool m_guardFlag = false;								//ガード：ガードのフラッグ。

	bool m_weakAtFlag = false;								//弱攻撃：弱攻撃中か。
	bool m_chargeAtFlag = false;							//溜め攻撃：溜め攻撃中か。
	float m_charge = 0.0f;									//溜め攻撃：チャージ量。

	float m_attackPower = 0.0f;								//攻撃共通：攻撃力。

	bool m_hitFlag = false;									//被弾：被弾中かのフラッグ。
	bool m_deathFlag = false;								//死亡：死亡しているかのフラッグ。


	AnimationClip m_animationClips[enPlayerAnimClip_Num];	//アニメーションクリップ。	
};

class IState
{
public:
	IState(Player*player) 
	{
		m_player = player;
	}
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

protected:
	Player* m_player = nullptr;
};

class StateMachine:public IGameObject
{
public:
	StateMachine();
	~StateMachine();
	bool Start()override;
	void Update()override;
	//ステート遷移（仮）。
	void StateManage();
private:
	Player* m_player = nullptr;
	float m_weakAtCT = 0.0f;								//弱攻撃クールタイム。
};

class PlayerIdle :public IState
{
public:
	PlayerIdle(Player* player)
		: IState(player)
	{
	}
	~PlayerIdle();
	void Enter()override;
	void Update()override;
	//待機。
	void idle();
	void Exit()override;
};

/// <summary>
/// 移動。
/// </summary>
class PlayerMove :public IState
{
public:
	PlayerMove(Player* player) 
		: IState(player)
	{
	}
	~PlayerMove();
	void Enter()override;
	void Update()override;
	//移動
	void AnimManage();
	//歩き。
	void Walk();
	//ダッシュ。
	void Dash();
	//ジャンプ。
	void Jump();
	void Exit()override;
private:
};

/// <summary>
/// 弱攻撃。
/// </summary>
class PlayerWeakAttack :public IState
{
public:
	PlayerWeakAttack(Player* player)
		: IState(player)
	{

	}
	~PlayerWeakAttack();
	void Enter()override;
	void Update()override;
	//ステート変更。
	void ChangeState();
	//弱攻撃。
	void WeakAttack();
	//コリジョン生成。
	void MakeCollision();			
	void Exit()override;
private:

};

/// <summary>
/// 溜め攻撃。
/// </summary>
class PlayerChargeAttack :public IState
{
public:
	PlayerChargeAttack(Player* player)
		: IState(player)
	{

	}
	~PlayerChargeAttack();
	void Enter()override;
	void Update()override;
	//チャージ蓄積。
	void Charging();				
	//攻撃。
	void ChargeAttack();			
	//コリジョン生成。
	void MakeCollision();			
	//ステート切り替え。
	void ChangeState();
	void Exit()override;

private:
	Vector3 m_RStickOld = Vector3::Zero;				//Rスティックの入力量（変更前）。
	float m_collisionSize = 0.0f;						//コリジョンサイズ。
	bool m_isCharging = true;							//チャージ中？
	bool m_isStateChange = false;						//アニメーションを切り替えた？	
};

/// <summary>
/// ガード。
/// </summary>
class PlayerGuard :public IState
{
public:
	PlayerGuard(Player* player)
		: IState(player)
	{

	}
	~PlayerGuard();

	void Enter()override;
	void Update()override;
	//ガードの方向を加味。
	void GuardDirection();						
	void Exit()override;

private:
	Vector3 m_directionGap = Vector3::Zero;				//向きの差分。
};


/// <summary>
/// 被弾。
/// </summary>
class PlayerHit :public IState
{
public:
	PlayerHit(Player* player)
		: IState(player)
	{

	}
	~PlayerHit();

	void Enter()override;
	void Update()override;
	//被弾の硬直。
	void ChangeState();
	void Exit()override;
private:
};

/// <summary>
/// 死亡。
/// </summary>
class PlayerDeath :public IState
{
public:
	PlayerDeath(Player* player)
		: IState(player)
	{

	}
	~PlayerDeath();

	void Enter()override;
	void Update()override;
	//ゲームオーバーへ移行。
	void ToGameOver();
	void Exit()override;
private:
};