#pragma once

namespace Character {
	class Player;
}
class EnemySpawn;
class EnemyBase;
class SoundEffect;

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
	//座標を設定
	void SetSnakePos(const Vector3& pos) { m_snakePos = pos; };
	//向きを設定
	void SetSnakeDir(const Vector3& dir) { m_snakeDir = dir; };
	//回転を設定
	void SetSnakeRot(const Quaternion& rot) { m_snakeRot = rot; };
	//スポーン状態を設定
	void SetSnakeIsSpawn(const bool& isSpawn) { m_isSpawn = isSpawn; };
	//キャラコンの位置を設定
	void SetSnakeCharaConPos(const Vector3& pos) { m_snakeController.SetPosition(pos); };

	//ゲッター
	//位置を取得
	Vector3 GetSnakePos() const { return m_snakePos; };
	//スポーン状態を取得
	bool GetIsSnakeSpawn() const { return m_isSpawn; };
private:

	CollisionObject*		collisionObject		= nullptr;
	Character::Player*		m_player			= nullptr;
	EnemySpawn*				m_enemySpawn		= nullptr;
	EnemyBase*				m_enemyBase			= nullptr;
  SoundEffect*        m_soundEffect = nullptr;	//サウンドエフェクト 

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
	float m_elapsedTime			= 0.0f;			//死亡経過時間

	int m_snakeState			= 0;			//ヘビのステート

	bool m_isCanStateChange		= true;			//ステートを変えてもよいか
	bool m_isSpawn				= false;			//敵が出現するか
	bool m_isRemoveController	= false;		//キャラコンを削除したか
};