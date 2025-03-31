#pragma once
namespace nsK2EngineLow {
	static const int MAX_POINT_LIGHT = 32;
	static const int MAX_SPOT_LIGHT = 32;

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
		void SetDirection(const Vector3& direction)
		{
			m_pos = direction;
			m_pos.Normalize();
		}
		void SetDirection(float x, float y, float z)
		{
			SetDirection({ x,y,z });
		}

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
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
		int		m_isUse = false;	//ライトの使用状況
		Vector3	m_color;			//色
		float	m_range;	//影響範囲

		/// <summary>
		/// ポイントライトの位置を設定
		/// </summary>
		/// <param name="direction"></param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
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
		}
		void SetColor(float x, float y, float z)
		{
			SetColor({ x,y,z });
		}

		/// <summary>
		/// 影響範囲の設定
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float& range)
		{
			m_range = range;
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
	/// スポットライトの構造体
	/// </summary>
	struct SSpotLight
	{
		Vector3 m_pos;				//位置
		int		m_isUse = false;	//ライトの使用状況
		Vector3 m_color;			//色
		float	m_range;			//影響範囲
		Vector3 m_direction;		//向き
		float	m_angle;			//射出角度

		/// <summary>
		/// ライトの位置を設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_pos = pos;
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
		}
		void SetColor(float x, float y, float z)
		{
			SetColor({ x,y,z });
		}

		/// <summary>
		/// 影響範囲の設定
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float& range)
		{
			m_range = range;
		}

		/// <summary>
		/// 向きの設定
		/// </summary>
		/// <param name="direction"></param>
		void SetDirection(const Vector3& direction)
		{
			m_direction = direction;
			m_direction.Normalize();
		}
		void SetDirection(float x, float y, float z)
		{
			SetDirection({ x,y,z });
		}

		/// <summary>
		/// 影響範囲の設定
		/// </summary>
		/// <param name="angle"></param>
		void SetAngle(const float angle)
		{
			m_angle = angle;
		}

		/// <summary>
		/// ポイントライトを使用中にする
		/// </summary>
		void Use()
		{
			m_isUse = true;
		}
	};

	/// <summary>
	/// 半球ライトの構造体
	/// </summary>
	struct SHemisphereLight
	{
		Vector3 m_groundColor;	//地面の色
		float pad0;
		Vector3 m_skyColor;		//空の色
		float pad1;
		Vector3 m_groundNormal;	//地面の法線

		/// <summary>
		/// 地面の色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetGroundColor(const Vector3& color) {
			m_groundColor = color;
		}
		void SetGroundColor(float x, float y, float z) {
			SetGroundColor({ x,y,z });
		}

		/// <summary>
		/// 空の色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetSkyColor(const Vector3& color) {
			m_skyColor = color;
		}
		void SetSkyColor(float x, float y, float z) {
			SetSkyColor({ x,y,z });
		}

		/// <summary>
		/// 地面の法線の設定
		/// </summary>
		/// <param name="normal"></param>
		void SetGroundNormal(const Vector3& normal) {
			m_groundNormal = normal;
			m_groundNormal.Normalize();
		}
		void SetGroundNormal(float x, float y, float z) {
			SetGroundNormal({ x,y,z });
		}
	};

	/// <summary>
	/// ライトの構造体
	/// </summary>
	struct Light {
		SDirectionLight m_drectionLight;					//シーンディレクションライト
		SPointLight		m_pointLight[MAX_POINT_LIGHT];		//ポイントライト
		SSpotLight		m_spotLight[MAX_SPOT_LIGHT];		//スポットライト
		SHemisphereLight m_hemisphereLight;					//半球ライト
		int				m_numPointLig = 0;					//ポイントライトの使用数
		Vector3			m_cameraPos = Vector3::Zero;		//カメラの向いている方向
		int				m_numSpotLig = 0;					//スポットライトの使用数
		Vector3			m_ambientLight = Vector3::Zero;		//環境光

		/// <summary>
		/// カメラの視点の位置の設定
		/// </summary>
		/// <param name="cameraPos"></param>
		void SetCameraPos()
		{
			m_cameraPos = g_camera3D->GetPosition();
		}

		/// <summary>
		/// 環境光の設定
		/// </summary>
		/// <param name="color">環境光の色</param>
		void SetAmbientLight(const Vector3& color)
		{
			m_ambientLight = color;
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

		SSpotLight* NewSpotLight();

		SHemisphereLight* GetHemisphereLight()
		{
			return &m_light.m_hemisphereLight;
		}

		//ライトのデータを取得
		Light* GetLightData()
		{
			return &m_light;
		}

		Light m_light;	//シーンライト
	private:

	};
}
