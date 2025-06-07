#pragma once
namespace Character {
	class Player;
}

namespace Enemy
{
	class EnemyBase : public IGameObject
	{
	public:
		bool Start() override;
		void Update() override;
		void Render(RenderContext& rc) override;

		///////////////////////////////////////////////
		// ゲッター
		///////////////////////////////////////////////

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

		///////////////////////////////////////////////
		// セッター
		///////////////////////////////////////////////

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


	protected:
		Character::Player* m_target = nullptr;		//ターゲットのインスタン

		Vector3			m_moveSpeed;		//移動ベクトル
		Vector3			m_position;			//位置
		Vector3			m_scale;			//拡大率
		Quaternion		m_rotation;			//向き

		ModelRender		m_enemyModel;		//モデル描画用

		CharacterController m_characterController;


	};
}