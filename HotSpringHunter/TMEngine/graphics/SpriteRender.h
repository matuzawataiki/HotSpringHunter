#pragma once


namespace nsTMEngine {
	class SpriteRender :public IRenderer
	{
	public:
		SpriteRender();
		~SpriteRender();
		/// <summary>
		/// 画像の初期化用関数
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="w">画像の横幅のサイズ</param>
		/// <param name="h">画像の縦幅のサイズ</param>
		/// <param name="alphaBlendMode"></param>
		void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
		/// <summary>
		/// 位置の設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector2& pos)
		{
			m_pos.x = pos.x;
			m_pos.y = pos.y;
			m_pos.x = 0.0f;
		}
		/// <summary>
		/// 位置の設定
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		void SetPosition(const float& x, const float& y)
		{
			m_pos = Vector3(x, y, 0.0f);
		}
		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rot = rot;
		}
		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="sca"></param>
		void SetScale(const Vector2& sca)
		{
			m_sca.x = sca.x;
			m_sca.y = sca.y;
			m_sca.x = 0.0f;
		}
		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot">ピボット。</param>
		void SetPivot(const Vector2& piv)
		{
			m_piv = piv;
		}
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() {
			m_sprite.Update(
				m_pos,
				m_rot,
				m_sca,
				m_piv
			);
		}
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

	private:
		void OnRender2D(RenderContext& rc)override
		{
			m_sprite.Draw(rc);
		}



	private:
		Sprite		m_sprite;						//スプライト
		Vector2		m_piv = Sprite::DEFAULT_PIVOT;	//ピボット
		Vector3		m_pos = Vector3::Zero;			//位置
		Vector3		m_sca = Vector3::One;			//大きさ
		Quaternion	m_rot = Quaternion::Identity;	//回転

	};
}

