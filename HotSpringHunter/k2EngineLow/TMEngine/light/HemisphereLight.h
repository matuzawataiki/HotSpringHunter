#pragma once

namespace nsK2EngineLow
{
	class HemisphereLight
	{
	public:
		HemisphereLight();
		~HemisphereLight();

		/// <summary>
		/// 半球ライトの初期設定
		/// </summary>
		void Init(const Vector3& groundColor, const Vector3& skyColor, const Vector3& groundNormal)
		{
			SetGroundColor(groundColor);
			SetSkyColor(skyColor);
			SetGroundNormal(groundNormal);
		}

		/// <summary>
		/// 地面の色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetGroundColor(const Vector3& color) {
			m_hemisphereLight->SetGroundColor(color);
		}
		void SetGroundColor(float x, float y, float z) {
			SetGroundColor({ x,y,z });
		}

		/// <summary>
		/// 空の色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetSkyColor(const Vector3& color) {
			m_hemisphereLight->SetSkyColor(color);
		}
		void SetSkyColor(float x, float y, float z) {
			SetSkyColor({ x,y,z });
		}

		/// <summary>
		/// 地面の法線の設定
		/// </summary>
		/// <param name="normal"></param>
		void SetGroundNormal(const Vector3& normal)
		{
			m_hemisphereLight->SetGroundNormal(normal);
		}
		void SetGroundNormal(float x, float y, float z) {
			SetGroundNormal({ x,y,z });
		}
	private:
		SHemisphereLight* m_hemisphereLight;	//半球ライト
	};
}
