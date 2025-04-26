#pragma once
class Towel;
class Bucket;

class SnakeEnemy;

//現在アクティブなステート。
//順番気をつけようね。
enum EnActiveState{
	enPlayerMove,
	enPlayerJump,
	enPlayerDash,
	enPlayerWeakAttack,
	enPlayerChargeAttack,
	enPlayeGuard,
	enPlayerHealth,
	enPlayerHit,
	enPlayerDeath,
};

//アニメーションステート。
enum EnPlayerAnimVar {
	enIdle,
	enWalk,
	enRun,
	enJump,
	enGuardStart,
	enGuardEnd,
	enWeakAttack,
	enChargeAttack,
	enCharging,
	enHit,
	enDeath,
};

//アニメーションクリップ。
enum EnAnimationClip {
	enAnimationClip_Idle,
	enAnimationClip_Walk,
	enAnimationClip_Run,
	enAnimationClip_Jump,
	enAnimationClip_GuardStart,
	enAnimationClip_GuardEnd,
	enAnimationClip_WeakAttack,
	enAnimationClip_ChargeAttack,
	enAnimationClip_Charging,
	enAnimationClip_Hit,
	enAnimationClip_Death,
	enAnimationClip_Num,
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
	//state管理。
	void StateManage();
	void Render(RenderContext& rc)override;

	//player座標のゲッター。
	Vector3 GetPlayerPos() {
		return m_playerPosition;
	}
	//playerの向きのゲッター。
	Vector3 GetPlayerDir() {
		return m_playerDirection;
	}

	std::vector<IState*>m_stateList;
	int m_currentState = 0;
	int m_requestState = 0;

	Towel* m_towel = nullptr;
	Bucket* m_bucket = nullptr;

	SnakeEnemy* m_snakeEnemy = nullptr;

	CollisionObject* m_collision = nullptr;


	CharacterController m_playerCharaCon;					//キャラコン。
	ModelRender m_playerModel;								//描画。
	Vector3 m_playerPosition = Vector3::Zero;				//座標。
	Vector3 m_playerSpeed = Vector3::Zero;					//移動スピード。
	Vector3 m_playerDirection = Vector3::Zero;				//向き。
	Quaternion m_playerRotation = Quaternion::Identity;		//回転。

	float m_dashState = 1.0f;								//走り状態の管理。
	float m_guardState = 1.0f;								//ガード状態の管理。
	float m_playerHP = 100.0f;								//player体力。
	bool m_guardFlag = false;				//ガードのフラッグ。


	int m_animationState = 0;								//アニメーションの状態。
	AnimationClip m_animationClips[enAnimationClip_Num];	//アニメーションクリップ。	
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

/// <summary>
/// 移動。
/// </summary>
class PlayerMove :public IState
{
public:
	PlayerMove(Player*player) 
		: IState(player)
	{
		
	}
	PlayerMove();
	~PlayerMove();
	void Enter()override;
	void Update()override;
	//移動
	void Move();										
	//向きを計算。
	void GetDirection(Vector3 foward, Vector3 right);
	//ステートで移動速度調整。
	void MoveAdjust();
	//回転。
	void Rotation();
	void Exit()override;
private:
};


/// <summary>
/// ダッシュ。
/// </summary>
class PlayerDash :public IState
{
public:
	PlayerDash(Player* player)
		: IState(player)
	{

	}
	PlayerDash();
	~PlayerDash();
	void Enter()override;
	void Update()override;
	//ダッシュ。
	void Dash();
	void Exit()override;
private:
};

/// <summary>
/// ジャンプ。
/// </summary>
class PlayerJump :public  IState
{
public:
	PlayerJump(Player* player)
		: IState(player)
	{

	}
	PlayerJump();
	~PlayerJump();
	void Enter()override;
	void Update()override;
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
	PlayerWeakAttack();
	~PlayerWeakAttack();
	void Enter()override;
	void Update()override;
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
	PlayerChargeAttack();
	~PlayerChargeAttack();
	void Enter()override;
	void Update()override;
	//チャージ蓄積。
	void StickCharge();				
	//攻撃。
	void ChargeAttack();			
	//コリジョン生成。
	void MakeCollision();			
	//チャージ量表示（仮）。
	void DisplayCharge();			
	void Exit()override;

	//チャージのゲッター。
	float GetCharge() {
		return m_charge;
	}
private:
	Vector3 m_RStickOld = Vector3::Zero;				//Rスティックの入力量（変更前）。
	float m_charge = 0.0f;								//チャージ量。
	float m_collisionSize = 0.0f;

	//チャージ量表示（仮）
	FontRender m_chargeRender;
	wchar_t m_chargeText[100];
	
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
	PlayerGuard();
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
/// HP管理。
/// </summary>
class PlayerHealth :public IState
{
public:
	PlayerHealth(Player* player)
		: IState(player)
	{

	}
	PlayerHealth();
	~PlayerHealth();
	void Enter()override;
	void Update()override;
	void Hit(float reduce);
	void TakeDamage();				//攻撃被弾。
	void DisplayHP();
	void Exit()override;

private:	
	//チャージ量表示（仮）
	FontRender m_HPRender;
	wchar_t m_HPText[100];
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
	PlayerHit();
	~PlayerHit();

	void Enter()override;
	void Update()override;
	//被弾。
	void Hit(float reduce);
	void Exit()override;
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
	PlayerDeath();
	~PlayerDeath();

	void Enter()override;
	void Update()override;
	//死亡判定。
	void DeathJudge();
	void Exit()override;
};