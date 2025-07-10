#pragma once
namespace Character {
	class Player;
}
class EnemyBase;
class GameCamera;
class EnemyHPBar;
class SoundEffect;
class BossHPUI;

//クマの行動状態
enum EnBearState {
	enBearContact,			//プレイヤーを認識
	enBearIdle,				//待機
	enBearTrack,			//追従
	enBearMeleeAttack,		//近接攻撃
	enBearCoverAttack,		//拘束攻撃
	enBearSlowPlayer,		//プレイヤーぶっ飛ばし攻撃
	enBearSlowStone,		//投石攻撃
	enBearSummonMinion,		//雑魚召喚
	enBearKnockBack,		//ノックバック
	enBearDeath,			//死亡
};

//クマのアニメーションクリップ
enum EnBearAnimClip
{
	enBearAnimClip_Idle,			//待機
	enBearAnimClip_Run,				//走り
	enBearAnimClip_NeleeAttack,		//ひっかき攻撃
	enBearAnimClip_Covering,		//覆いかぶさる
	enBearAnimClip_SlowStone,		//投石攻撃
	enBearAnimClip_Roar,			//咆哮
	enBearAnimClip_Hit,				//被弾
	enBearAnimClip_Death,			//死亡
	enBearAnimClip_Num,				
};

class Bear :public IGameObject
{
public:
	Bear();
	~Bear();
	bool Start()override;

	/// <summary>
	/// アセットを読み込みます。
	/// </summary>
	void LoadAssets();

	void Update()override;

	/// <summary>
	/// 投石攻撃
	/// </summary>
	void StoneThrow();

	/// <summary>
	/// 投石のベクトルを計算
	/// </summary>
	/// <param name="start">投石の開始位置</param>
	/// <param name="target">投石の目標位置</param>
	/// <param name="t">投石の経過時間</param>
	/// <returns>岩の現在位置</returns>
	Vector3 CalcStoneVec(Vector3& start, Vector3& target, const float t);

	/// <summary>
	/// 岩のコリジョンを作る
	/// </summary>
	void StoneCollision();

	/// <summary>
	/// 雑魚を召喚
	/// </summary>
	void SummonMinions();	

	/// <summary>
	/// 召喚する位置を計算
	/// </summary>
	void CalcPos();

	/// <summary>
	/// 死亡後に下に沈ませる
	/// </summary>
	void SinkIntoGround();

	/// <summary>
	/// 被弾エフェクトを再生
	/// </summary>
	void PlayHitEffect();

	/// <summary>
	/// 咆哮エフェクトを再生
	/// </summary>
	void PlayRoarEffect();

	/// <summary>
	/// ステート管理
	/// </summary>	
	void ManageState();

	/// <summary>
	/// プレイヤーを探す
	/// </summary>
	void FindPlayer();

	/// <summary>
	/// 行動実行
	/// </summary>
	void ExecuteAction();

	/// <summary>
	/// いろいろ更新
	/// </summary>
	void VariousUpdate();

	/// <summary>
	/// クマの向きを更新
	/// </summary>
	void DirUpdate();

	/// <summary>
	/// 速度を適応
	/// </summary>
	void ExecuteSpeed();

	/// <summary>
	/// 描画更新
	/// </summary>
	void Render(RenderContext& rc)override;


	//定数のゲッター

	/// <summary>
	/// クマの最大HPのゲッター
	/// </summary>
	float GetBearMAXHP();

	/// <summary>
	/// 拘束攻撃の初期の時間のゲッター
	/// </summary>
	float GetCOVER_TIME();

	/// <summary>
	/// ぶっ飛ばしの距離のゲッター
	/// </summary>
	float GetBLOW_POS_DIS();

	//セッター
	//位置を設定
	inline void SetBearPos(const Vector3& pos) { m_bearPos = pos; };
	//向きを設定
	inline void SetBearDir(const Vector3& dir) { m_bearDir = dir; };
	//回転を設定
	inline void SetBearRot(const Quaternion& rot) { m_bearRot = rot; };
	//キャラコンの位置を設定
	inline void SetBearCharaConPos(const Vector3& pos) { m_bearController.SetPosition(pos); };
	//スポーン位置を設定
	inline void SetBearNewPos(const Vector3& pos) { m_bearNewPos = pos; };
	//拘束中フラッグを設定
	inline void SetIsCovering(const bool isCovering) { m_isCovering = isCovering; };
	//ぶっ飛ばし攻撃フラッグを設定
	inline void SetIsSlowPlayer(const bool isSlow) { m_isSlowPlayer = isSlow; };

	//ゲッター	
	//位置を取得
	inline Vector3 GetBearPos() const { return m_bearPos; };
	//向きを取得
	inline Vector3 GetBearDir() const { return m_bearDir; };
	//HPを取得
	inline float GetBearHP() const { return m_bearHP; };
	//拘束中フラッグを取得
	inline bool GetIsCovering() const { return m_isCovering; };
	//ぶっ飛ばし攻撃フラッグを取得
	inline bool GetIsSlowPlayer() const { return m_isSlowPlayer; };

private:
	Character::Player*	m_player			= nullptr;
	EnemyBase*			m_enemyBase			= nullptr;
	CollisionObject*	m_stoneCollision	= nullptr;
	CollisionObject*	m_coverCollision	= nullptr;
	GameCamera*			m_gameCamera		= nullptr;
	SoundEffect*        m_soundEffect       = nullptr;
	BossHPUI*			m_bossHPUI			= nullptr;

	std::vector<Vector3>m_summonPos;				//召喚する位置

	ModelRender				m_bearModel;							//モデルレンダー
	ModelRender				m_stoneModel;							//投石攻撃の岩のモデル
	SpriteRender			m_RStickImage;							//Rスティックのイラスト
	CharacterController		m_bearController;						//キャラクターコントローラー
	AnimationClip			m_animationClips[enBearAnimClip_Num];	//アニメーションクリップ
	Quaternion				m_bearRot	= Quaternion::Identity;		//回転
	Quaternion				m_stoneRot	= Quaternion::Identity;		//岩の回転

	Vector3 m_bearPos			= Vector3::Zero;		//座標
	Vector3 m_bearSpeed			= Vector3::Zero;		//敵の速度
	Vector3 m_bearDir			= Vector3::Zero;		//向き
	Vector3 m_toPlayer			= Vector3::Zero;		//ベクトル
	Vector3 m_stonePos			= Vector3::Zero;		//岩の位置
	Vector3 m_stoneSpeed		= Vector3::Zero;		//岩の速度
	Vector3 m_stoneDir			= Vector3::Zero;		//岩の向き
	Vector3 m_newStonePos		= Vector3::Zero;		//投石をセットする位置
	Vector3 m_toSlowPos			= Vector3::Zero;		//投石の目標位置
	Vector3 m_bearNewPos		= Vector3::Zero;		//クマのスポーン位置
	Vector3 m_bearMoveAmount	= Vector3::Zero;		//1フレームのクマの移動量

	float m_bearHP				= 0.0f;		//敵のHP
	float m_ATKCoolTime			= 0.0f;		//攻撃のクールタイム
	float m_slowCoolTime		= 0.0f;		//投石攻撃のクールタイム
	float m_summonCoolTime		= 0.0f;		//召喚のクールタイム
	float m_coverCoolTime		= 0.0f;		//拘束攻撃のクールタイム
	float m_setStoneTime		= 0.0f;		//投石攻撃の準備時間
	float m_flightTime			= 0.0f;		//投石の飛行経過時間
	float m_coverTime			= 0.0f;		//拘束攻撃の攻撃判定を出す時間
	float m_damageMemory		= 0.0f;		//被ダメージ量を記憶する

	int m_bearState				= 0;		//クマの行動状態

	bool m_isCanStateChange		= false;	//ステート変更を受け付けているか
	bool m_isContact			= false;	//プレイヤーを認識したか
	bool m_isPlayDeadAnim		= false;	//死亡アニメーションを再生したか
	bool m_isRemoveController	= false;	//キャラコンを削除したか
	bool m_isSetStone			= false;	//投石：岩の準備をしたか
	bool m_isStoneSlowing		= false;	//投石：岩が飛ばされているか
	bool m_isStoneDraw			= false;	//投石：岩を描画するか
	bool m_isSummon				= false;	//召喚：雑魚を召喚したか
	bool m_isSummonEnd			= false;	//召喚：雑魚召喚が終わったか
	bool m_isPlayRoar			= false;	//咆哮アニメーションを再生したか
	bool m_isPutCoverCollision	= false;	//拘束攻撃の当たり判定を出したか
	bool m_isCovering			= false;	//プレイヤーを拘束中か
	bool m_isOutCovering		= false;	//拘束攻撃から抜け出したか
	bool m_isSlowPlayer			= false;	//ぶっ飛ばしを行うか
	bool m_isPlayerCover		= false;	//プレイヤーが拘束状態か
};