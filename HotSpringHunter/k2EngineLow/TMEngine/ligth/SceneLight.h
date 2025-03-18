#pragma once
#include "DirectionLight.h"
namespace nsK2EngineLow {

	struct SDirectionLight
	{
		Vector3 m_direction;	//方向
		float pad;
		Vector3 m_color;		//色

		/// <summary>
		/// ディレクションライトの方向を設定
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
		/// 色の設定
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color) {
			m_color = color;
			m_color.Normalize();
		}
		void SetColor(float x, float y, float z) {
			SetColor({ x,y,z });
		}
	public:

	};

	struct Light {
		SDirectionLight m_sDrectionLight;		//シーンディレクションライト
	};

	class SceneLight
	{
	public:
		SceneLight();
		~SceneLight();

		void Init();

		//ライトのデータを取得
		Light* GetLightData()
		{
			return &m_light;
		}

		Light m_light;	//シーンライト
	private:

	};
}
