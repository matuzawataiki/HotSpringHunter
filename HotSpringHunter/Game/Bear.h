#pragma once

namespace Character {
	class Player;
}
class Player;
class SnakeEnemy;
class EnemySpawn;
class EnemyBase;
class EnemyManager;
class GameCamera;
class EnemyHPBar;

//クマの行動状態
enum EnBearState {
	enbearContact,			//プレイヤーを認識
	enBearIdle,				//待機
	enBearTrack,			//追従
	enBearMeleeAttack,		//近接攻撃
	enBearSlowStone,		//投石攻撃
	enBearGoNewPos,			//初期位置へ移動（召喚用）
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
	//アセットを読み込む
	void LoadAssets();
	void Update()override;
	//投石攻撃
	void StoneThrow();
	//投石のベクトルを計算
	Vector3 CalcStoneVec(Vector3 start, Vector3 target, float t);
	//岩のコリジョンを作る
	void StoneCollision();
	//クマを初期位置に移動(召喚用)
	void GoNewPos();
	//雑魚召喚
	void SummonMinions();	
	//召喚する位置を計算
	void CalcPos();
	//死亡後に下に沈ませる
	void SinkIntoGround();
	//ステート管理
	void ManageState();
	//プレイヤーを探す
	void FindPlayer();
	//行動実行
	void ExecuteAction();
	//いろいろ更新
	void VariousUpdate();
	//クマの向きを更新
	void DirUpdate();
	//速度を適応
	void ExecuteSpeed();
	//クマのHPのゲッター
	float GetBearMAXHP();
	void Render(RenderContext& rc)override;

	//セッター
	//位置を設定
	void SetBearPos(const Vector3& pos) { m_bearPos = pos; };
	//向きを設定
	void SetBearDir(const Vector3& dir) { m_bearDir = dir; };
	//回転を設定
	void SetBearRot(const Quaternion& rot) { m_bearRot = rot; };
	//スポーン状態を設定
	void SetBearIsSpawn(const bool& isSpawn) { m_isSpawn = isSpawn; };
	//キャラコンの位置を設定
	void SetBearCharaConPos(const Vector3& pos) { m_bearController.SetPosition(pos); };
	//スポーン位置を設定
	void SetBearNewPos(const Vector3& pos) { m_bearNewPos = pos; };

	//ゲッター	
	//位置を取得
	Vector3 GetBearPos() const { return m_bearPos; };
	//HPを取得
	float GetBearHP() const { return m_bearHP; };
	//スポーン状態を取得
	bool GetIsBearSpawn() const { return m_isSpawn; };

private:
	Character::Player*	m_player			= nullptr;
	SnakeEnemy*			m_snakeEnemy[4]		= {};
	EnemySpawn*			m_enemySpawn		= nullptr;
	EnemyBase*			m_enemyBase			= nullptr;
	EnemyManager*		m_enemyManager		= nullptr;
	CollisionObject*	m_stoneCollision	= nullptr;
	GameCamera*			m_gameCamera		= nullptr;

	std::vector<Vector3>m_summonPos;				//召喚する位置

	ModelRender				m_bearModel;							//モデルレンダー
	ModelRender				m_stoneModel;							//投石攻撃の岩のモデル
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

	float m_bearHP				= 0.0f;		//敵のHP
	float m_ATKCoolTime			= 0.0f;		//攻撃のクールタイム
	float m_slowCoolTime		= 0.0f;		//投石攻撃のクールタイム
	float m_setStoneTime		= 0.0f;		//投石攻撃の準備時間
	float m_flightTime			= 0.0f;		//投石の飛行経過時間

	int m_bearState				= 0;		//クマの行動状態

	bool m_isCanStateChange		= true;		//ステート変更を受け付けているか
	bool m_isSpawn				= false;	//敵がスポーンしているか
	bool m_isContact			= false;	//プレイヤーを認識したか
	bool m_isPlayDeadAnim		= false;	//死亡アニメーションを再生したか
	bool m_isRemoveController	= false;	//キャラコンを削除したか
	bool m_isSetStone			= false;	//投石：岩の準備をしたか
	bool m_isStoneSlowing		= false;	//投石：岩が飛ばされているか
	bool m_isStoneDraw			= false;	//投石：岩を描画するか
	bool m_isSummon				= false;	//召喚：雑魚を召喚したか
	bool m_isSummonEnd			= false;	//召喚：雑魚召喚が終わったか
	bool m_isPlayRoar			= false;	//咆哮アニメーションを再生したか
};