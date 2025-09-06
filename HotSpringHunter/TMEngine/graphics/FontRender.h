#pragma once
#include "IRenderer.h"

namespace nsTMEngine
{
	class FontRender : public IRenderer
	{
	public:
		static const int MAX_TEXT_SIZE = 256;
		~FontRender()
		{

		}
		/// <summary>
		/// 文字の設定
		/// </summary>
		/// <param name="text"></param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		/// <summary>
		/// 座標を設定。zは0.0fで。
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(float x, float y)
		{
			SetPosition({ x, y, 0.0f });
		}
		/// <summary>
		/// 大きさを設定。zは0.0fで。
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
		}
		/// <summary>
		/// 大きさを設定。
		/// </summary>
		/// <param name="scale">大きさ。</param>
		void SetScale(const float sca)
		{
			m_sca = sca;
		}

		/// <summary>
		/// 色を設定。
		/// </summary>
		/// <param name="color">色。</param>
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r, g, b, a });
		}
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rotation">回転。</param>
		void SetRotation(const float rot)
		{
			m_rot = rot;
		}
		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot">
		/// ピボット。
		/// x = 0.5, y = 0.5で画像の中心が基点。
		/// x = 0.0, y = 0.0で画像の左下。
		/// x = 1.0, y = 1.0で画像の右上。
		/// UnityのuGUIに準拠。
		/// </param>
		void SetPivost(const Vector2& piv)
		{
			m_piv = piv;
		}
		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc">レンダ―コンテキスト。</param>
		void Draw(RenderContext& rc);

	private:
		void OnRender2D(RenderContext& rc)override
		{
			m_font.Begin(rc);
			m_font.Draw(m_text, Vector2(m_pos.x, m_pos.y), m_color, m_rot, m_sca, m_piv);
			m_font.End(rc);
		}

	private:
		Vector3		m_pos = Vector3::Zero;			//位置
		float		m_sca = 1.0f;					//大きさ
		Vector4		m_color = g_vec4White;			//色
		float		m_rot = 0.0f;					//回転
		Vector2		m_piv = Sprite::DEFAULT_PIVOT;	//ピボット
		wchar_t		m_text[MAX_TEXT_SIZE];			//文字
		Font		m_font;							//フォント
	};
}