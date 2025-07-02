#pragma once
#include "Item/PowerUpBox.h"

class SnakeEnemy;
class SoundEffect;
class EnemyManager;
class GameCamera;
class SceneManager;
class Bear;
class SoundEffect;


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
		enPlayerToGoal,				//ゴール地点へ向かう
		enPlayerRestrain,			//拘束されている
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
		enPLayerAnimClip_Restrain,
		enPlayerAnimClip_Hit,
		enPlayerAnimClip_Death,
		enPlayerAnimClip_Num,
	};

	enum EnChargeState {
		enCharge01,
		enCharge02,
		enCharge03,
		enNone,
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
		void GetPowerUp(PowerUpBox::EnPowerUp powerUp);
		const int GetPowerUpSelect() { return m_mainPowerUp; }
		const int GetUpgradeSelect() { return m_subPowerUp; }

		//ゲッター
		//座標を取得
		inline Vector3 GetPlayerPos() const { return m_playerPos; };
		//向きを取得
		inline Vector3 GetPlayerDir() const { return m_playerDir; };
		//回転を取得
		inline Quaternion GetPlayerRot() const { return m_playerRot; };
		//攻撃力を取得
		inline float GetAttackPower() const { return m_attackPower; };
		//チャージ量を取得
		inline float GetCharge() const { return m_charge; };
		//現在のHPを取得
		inline float GetPlayerHP() const { return m_playerHP; };
		//行動状態を取得
		inline int GetPlayerState() const { return m_currentState; };

		//セッター
		//位置を設定
		inline void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; };
		//キャラコンの位置を設定
		inline void SetPlayerControllerPos(const Vector3& pos) { m_playerCharaCon.SetPosition(pos); };
		//向きを設定
		inline void SetPlayerDir(const Vector3& dir) { m_playerDir = dir; };
		//速度を設定
		inline void SetPlayerSpeed(const Vector3& speed) { m_playerSpeed = speed; };
		//行動状態を設定
		inline void SetRequestState(const int state) { m_requestState = state; };

	//private:
		//チャージ量表示（仮）
		FontRender m_chargeRender;
		wchar_t m_chargeText[100];
		//座標表示（仮）。
		FontRender m_posRender;
		wchar_t m_posText[256];

		std::vector<IState*> m_stateList;	//各ステートクラスのリスト。
		int m_currentState = 0;				//現在のステート。
		int m_requestState = 0;				//変更したいステート。
		int m_mainPowerUp = 2;				//メインのパワーアップ。1,遠距離攻撃。2,範囲攻撃
		int m_subPowerUp = 2;				//サブのアップデート


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
		float m_damageMemory		= 0.0f;				//受けたダメージを記録
		float m_dashState			= 1.0f;				//ダッシュ：走り状態の移動管理。
		float m_guardState			= 1.0f;				//ガード：ガード状態の移動管理。
		float m_charge				= 0.0f;				//溜め攻撃：チャージ量。
		float m_attackPower			= 0.0f;				//攻撃共通：攻撃力。

		bool m_guardFlag			= false;			//ガード：ガードのフラッグ。
		bool m_weakAtFlag			= false;			//弱攻撃：弱攻撃中か。
		bool m_chargeAtFlag			= false;			//溜め攻撃：溜め攻撃中か。
		bool m_hitFlag				= false;			//被弾：被弾中かのフラッグ。
		bool m_isDead				= false;			//死亡：死亡しているかのフラッグ。
		bool m_isDrawRStick			= false;
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
		Player*			m_player		= nullptr;
		SoundEffect*	m_soundEffect	= nullptr;	//サウンドソース。
		GameCamera*		m_gameCamera	= nullptr;
		SceneManager*	m_sceneManager	= nullptr;
		Bear*			m_bear			= nullptr;

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
		//チャージ状態を変更。
		void ChangeChargeState();
		//攻撃。
		void ChargeAttack();
		//コリジョン生成。
		void MakeCollision();
		//ステート切り替え。
		void ChangeState();
		void Exit()override;

	private:
		SoundEffect* m_soundEffect	= nullptr;			//サウンドソース。
		CollisionObject* m_chargeCollision = nullptr;	//チャージ攻撃中のコリジョン
		
		Vector3		m_RStickOld		= Vector3::Zero;	//Rスティックの入力量（変更前）。
		float		m_collisionSize = 0.0f;				//コリジョンサイズ
		int			m_chargeState	= enNone;			//チャージ状態。
		int			m_chargeOldState = enNone;			//チャージ状態（変更前）。
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

	/// <summary>
	/// ゴール地点へ移動
	/// </summary>
	class PlayerToGoal :public IState
	{
	public:
		PlayerToGoal(Player* player)
			: IState(player)
		{

		}
		~PlayerToGoal();
		void Enter()override;
		void Update()override;
		//移動量を計算
		void CalcMoveAmount();
		//プレイヤーをゴール地点へ移動させる
		void MoveToGoalPos();
		
		void Exit()override;

	private:
		SceneManager*	m_sceneManager	= nullptr;

		Vector3		m_moveAmount		= Vector3::Zero;		//1フレームで移動する量

		float		m_clearElapsedTime	= 0.0f;			//ゴールへの強制移動を始めてからの経過時間
	};


	class PlayerRestrain :public IState
	{
	public:
		PlayerRestrain(Player* player)
			: IState(player)
		{

		}
		~PlayerRestrain();

		void Enter()override;
		void Update()override;
		//拘束中の処理
		void Restraining();
		//レバガチャ
		void StickSpin();
		//ぶっ飛ばされる
		void HasBlowing(Vector3& start, Vector3& target, const float timeRatio);
		void Exit()override;

	private:
		Bear*		m_bear				= nullptr;

		Vector3		m_RStickOld			= Vector3::Zero;		//変更前のスティック入力量
		Vector3		m_blowStartPos	= Vector3::Zero;		//拘束されたときのプレイヤーの位置
		Vector3		m_blowTargetPos		= Vector3::Zero;

		float		m_power				= 0.0f;					//レバガチャのパワー
		float		m_restCoverTime		= 0.0f;					//残り拘束時間
		float		m_elapsedTime		= 0.0f;					//ぶっ飛ばされてからの経過時間
	};
}
