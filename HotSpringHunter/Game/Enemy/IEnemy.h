#pragma once
namespace Character {
	class Player;
}

namespace Enemy
{
	// 基底となるステータス
	// 各Enemyで必要なものが増えたら継承して使ってください。
	struct IEnemyStatus
	{
		float m_hp;
		float m_attackCoolTime;
		//
		IEnemyStatus()
			: m_hp(0.0f)
			, m_attackCoolTime(0.0f)
		{
		}
	};

	class IEnemy : public IGameObject
	{
	public:
		IEnemy();
		virtual ~IEnemy() {}
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;

		virtual void SetupStatus() = 0;

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 位置の取得
		/// </summary>
		/// <returns>位置</returns>
		inline const Vector3& GetPosition() {
			return m_position;
		}
		/// <summary>
		/// 拡大率の取得
		/// </summary>
		/// <returns>拡大率</returns>
		inline const Vector3& GetScale() {
			return m_scale;
		}
		/// <summary>
		/// 移動ベクトルの取得
		/// </summary>
		/// <returns>移動ベクトル</returns>
		inline const Vector3& GetMoveSpeed() {
			return m_moveSpeed;
		}
		/// <summary>
		/// 向きの取得
		/// </summary>
		/// <returns>向き</returns>
		inline const Quaternion& GetRotation() {
			return m_rotation;
		}
		/// <summary>
		/// ターゲットのインスタンスを取得
		/// </summary>
		/// <returns>ターゲット</returns>
		inline Character::Player* GetTarget() {
			return m_target;
		}
		/// <summary>
		/// モデルレンダーの取得
		/// </summary>
		/// <returns>モデルレンダー</returns>
		inline ModelRender* GetModelRender() {
			return &m_enemyModel;
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 位置の設定
		/// </summary>
		/// <param name="position">位置</param>
		inline void SetPosition(const Vector3& position) {
			m_position = position;
		}

		/// <summary>
		/// 拡大率の設定
		/// </summary>
		/// <param name="scale">拡大率</param>
		inline void SetScale(const Vector3& scale) {
			m_scale = scale;
		}

		/// <summary>
		/// 移動速度の設定
		/// </summary>
		/// <param name="moveSpeed">移動速度</param>
		inline void SetMoveSpeed(const Vector3& moveSpeed) {
			m_moveSpeed = moveSpeed;
		}

		/// <summary>
		/// 向きの設定
		/// </summary>
		/// <param name="rotetion">向き</param>
		inline void SetRotetion(const Quaternion& rotetion) {
			m_rotation = rotetion;
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 攻撃のクールタイムが明けているかどうか
		/// </summary>
		/// <returns>攻撃可能ならture、不可能ならfalse</returns>
		inline bool IsAttackCooldown() {
			return m_isAttackCooldown;
		 }

		/// <summary>
		/// 自分に攻撃がヒットしたかどうか
		/// </summary>
		/// <returns>ヒットしたならture、してないならfalse</returns>
		inline bool IsHit() {
			return m_isHit;
		}

		/// <summary>
		/// 死んでいるかどうか
		/// </summary>
		/// <returns>死んでいるならture、生きているならfalse</returns>
		inline bool IsDeath() {
			return m_isDeath;
		}

		//////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// ダメージを与える
		/// </summary>
		/// <param name="damage">ダメージ量</param>
		inline void AddDamage(float damage) {
			m_hp -= damage;
		}

		/// <summary>
		/// ヒットフラグを変更する
		/// </summary>
		inline void ChangeHitFlag() {
			m_isHit = !m_isHit;
		}
		
		/// <summary>
		/// エネミーを削除する
		/// </summary>
		inline void EnemyDeath() {
			DeleteGO(this);
		}

	protected:
		Character::Player* m_target = nullptr;		//ターゲットのインスタン

		Vector3			m_moveSpeed	= Vector3::Zero;		//移動ベクトル
		Vector3			m_position	= Vector3::Zero;		//位置
		Vector3			m_scale		= Vector3::Zero;		//拡大率
		Quaternion		m_rotation	= Quaternion::Identity;	//向き

		ModelRender		m_enemyModel;	//モデル描画用

		CharacterController m_characterController;

		IEnemyStatus* m_status;

		float m_hp		= 1.0f;				//体力
		float m_hpMax	= 1.0f;				//最大体力
		float m_attackTime = 10.0f;			//攻撃のクールタイム

		bool m_isAttackCooldown = false;	//攻撃フラグ
		bool m_isHit			= false;	//ヒットフラグ
		bool m_isDeath			= false;	//死亡フラグ

	};
}