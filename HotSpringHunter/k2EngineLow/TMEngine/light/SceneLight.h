#pragma once
namespace nsK2EngineLow {
	static const int MAX_POINT_LIGHT = 32;

	/// <summary>
	/// ディレクションライトの構造体
	/// </summary>
	struct SDirectionLight
	{
		Vector3 m_pos;		//方向
		float pad0;
		Vector3 m_color;	//色
		float pad1;

		/// <summary>
		/// ディレクションライトの方向を設定
		/// </summary>
		/// <param name="direction"></param>
		void SetPosition(const Vector3& direction)
		{
			m_pos = direction;
			m_pos.Normalize();
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
			m_color.Normalize();
		}
		void SetColor(float x, float y, float z)
		{
			SetColor({ x,y,z });
		}
	public:

	};
	/// <summary>
	/// ポイントライトの構造体
	/// </summary>
	struct SPointLight
	{
		Vector3 m_pos;				//方向
		int		m_isUse = false;	//ポイントライトの使用状況
		Vector3	m_color;			//色
		float	m_influenceRange;	//影響範囲

		/// <summary>
		/// ポイントライトの位置を設定
		/// </summary>
		/// <param name="direction"></param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
			m_pos.Normalize();
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
			m_color.Normalize();
		}
		void SetColor(float x, float y, float z)
		{
			SetColor({ x,y,z });
		}
		/// <summary>
		/// 影響範囲の設定
		/// </summary>
		/// <param name="color"></param>
		void SetRange(const float& influenceRange)
		{
			m_influenceRange = influenceRange;
		}
		/// <summary>
		/// ポイントライトを使用中にする
		/// </summary>
		void Use()
		{
			m_isUse = true;
		}
	public:

	};
	/// <summary>
	/// ライトの構造体
	/// </summary>
	struct Light {
		SDirectionLight m_drectionLight;						//シーンディレクションライト
		SPointLight		m_pointLight[MAX_POINT_LIGHT];			//ポイントライト
		int				m_numPointLig = 0;						//ポイントライトの使用数
		const Vector3*	m_cameraPos = nullptr;					//カメラの向いている方向
		float pad0;
		Vector3			m_ambientLight;							//環境光

		/// <summary>
		/// カメラの視点の位置の設定
		/// </summary>
		/// <param name="cameraPos"></param>
		void SetCameraPos()
		{
			m_cameraPos = g_camera3D->GetPositionPinta();
		}
		/// <summary>
		/// 環境光の設定
		/// </summary>
		/// <param name="color">環境光の色</param>
		void SetAmbientLight(const Vector3& color)
		{
			m_ambientLight = color;
			m_ambientLight.Normalize();
		}
		void SetAmbientLight(float x,float y,float z)
		{
			SetAmbientLight({ x,y,z });
		}
	};

	class SceneLight
	{
	public:
		SceneLight();
		~SceneLight();

		void Init();

		SPointLight* NewPointLight();

		//ライトのデータを取得
		Light* GetLightData()
		{
			return &m_light;
		}

		Light m_light;	//シーンライト
	private:

	};
}
