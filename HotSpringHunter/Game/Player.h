#pragma once


class SnakeEnemy;
class SoundEffect;
class EnemyManager;
class GameCamera;

namespace Character {
	//現在アクティブなステート。
	//順番気をつけようね。
	enum EnPlayerActiveState {
		enPlayerIdle,				//待機。
		enPlayerWalk,				//移動。
		enPlayerWeakAttack,			//弱攻撃。
		enPlayerChargeAttack,		//溜め攻撃。
		enPlayerGuard,				//ガード。
		enPlayerHit,				//被弾。
		enPlayerDeath,				//死亡。
	};

	//アニメーションクリップ。
	enum EnPlayerAnimClip {
		enPlayerAnimClip_Idle,
		enPlayerAnimClip_Walk,
		enPlayerAnimClip_Run,
		enPlayerAnimClip_Jump,
		enPlayerAnimClip_GuardStart,
		enPlayerAnimClip_WeakAttack,
		enPlayerAnimClip_ChargeAttack,
		enPLayerAnimClip_Charging,
		enPlayerAnimClip_Hit,
		enPlayerAnimClip_Death,
		enPlayerAnimClip_Num,
	};

	
	class IState;
	class StateMachine;

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
		void Hit(const float reduce);
		//state管理。
		void StateManage();
		//チャージ量表示（仮）。
		void DisplayCharge();
		//座標表示（仮）。
		void PositionDraw();
		void Render(RenderContext& rc)override;
		//プレイヤーの最大HPを取得。
		float GetPlayerMAXHP();

		////セッター
		////リクエストステートを設定
		//inline void SetRequestState

		//ゲッター
		//player座標を取得
		inline Vector3 GetPlayerPos() const { return m_playerPos; };
		//playerの向きを取得
		inline Vector3 GetPlayerDir() const { return m_playerDir; };
		//攻撃力のを取得
		inline float GetAttackPower() const { return m_attackPower; };
		//チャージのを取得
		inline float GetCharge() const { return m_charge; };
		//HPのを取得
		inline float GetPlayerHP() const { return m_playerHP; };

		//セッター
		//位置を設定
		inline void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; };
	//private:
		//チャージ量表示（仮）
		FontRender m_chargeRender;
		wchar_t m_chargeText[100];
		//座標表示（仮）。
		FontRender m_posRender;
		wchar_t m_posText[256];

		std::vector<IState*> m_stateList;			//各ステートクラスのリスト。
		int m_currentState = 0;						//現在のステート。
		int m_requestState = 0;						//変更したいステート。

		StateMachine*		m_stateMachine	= nullptr;
		CollisionObject*	m_collision		= nullptr;
		SoundEffect*		m_soundEffect	= nullptr;

		AnimationClip		m_animationClips[enPlayerAnimClip_Num];	//アニメーションクリップ。	
		CharacterController m_playerCharaCon;						//キャラコン。
		ModelRender			m_playerModel;							//描画。
		Quaternion			m_playerRot		= Quaternion::Identity;	//回転。

		Vector3	m_playerPos			= Vector3::Zero;	//座標。
		Vector3 m_playerSpeed		= Vector3::Zero;	//移動スピード。
		Vector3 m_playerDir			= Vector3::Zero;	//向き。

		float m_playerHP			= 0.0f;				//体力。
		float m_dashState			= 1.0f;				//ダッシュ：走り状態の移動管理。
		float m_guardState			= 1.0f;				//ガード：ガード状態の移動管理。
		float m_charge				= 0.0f;				//溜め攻撃：チャージ量。
		float m_attackPower			= 0.0f;				//攻撃共通：攻撃力。

		bool m_guardFlag			= false;			//ガード：ガードのフラッグ。
		bool m_weakAtFlag			= false;			//弱攻撃：弱攻撃中か。
		bool m_chargeAtFlag			= false;			//溜め攻撃：溜め攻撃中か。
		bool m_hitFlag				= false;			//被弾：被弾中かのフラッグ。
		bool m_isDead				= false;			//死亡：死亡しているかのフラッグ。
	};

	class IState
	{
	public:
		IState(Player* player)
		{
			m_player = player;
		}
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;

	protected:
		Player* m_player = nullptr;
	};

	class StateMachine :public IGameObject
	{

	public:
		StateMachine();
		~StateMachine();
		bool Start()override;
		void Update()override;
		//ステート遷移（仮）。
		void StateManage();
	private:
		Player*			m_player	  = nullptr;
		SoundEffect*	m_soundEffect = nullptr;	//サウンドソース。
		GameCamera*		m_gameCamera  = nullptr;

    
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
	private:
		SoundEffect* m_soundEffect = nullptr;
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
		//ダッシュ。
		void Dash();
		void Exit()override;
	private:
		SoundEffect* m_soundEffect = nullptr;	//サウンドソース。
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
		//最寄りの敵を自動ロックオン
		void LockOnEnemy();
		//最寄りの敵に吸いつき
		void SuctionEnemy();
		//コリジョン生成。
		void MakeCollision();
		void Exit()override;
	private:
		SoundEffect* m_soundEffect		= nullptr;			//サウンドソース。
		EnemyManager* m_enemyManager	= nullptr;

		Vector3 m_toSuctionTarget		= Vector3::Zero;	//吸いつきの目標位置

		float m_suctionElapsedTime		= 0.0f;				//吸いつきの経過時間

		bool m_isSuctionDecide			= false;			//敵への吸いつきを行うか
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
		SoundEffect* m_soundEffect	= nullptr;			//サウンドソース。
  
		Vector3		m_RStickOld		= Vector3::Zero;	//Rスティックの入力量（変更前）。
		float		m_collisionSize = 0.0f;				//コリジョンサイズ。
		bool		m_isCharging	= true;				//チャージ中？
		bool		m_isStateChange = false;			//アニメーションを切り替えた？	
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
	SoundEffect* m_soundEffect	= nullptr;	//サウンドソース。

	Vector3 m_directionGap		= Vector3::Zero;				//向きの差分。
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
		SoundEffect* m_soundEffect = nullptr;	//サウンドソース。
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
		void Exit()override;
	private:
		SoundEffect* m_soundEffect = nullptr;	//サウンドソース。
	};
}
