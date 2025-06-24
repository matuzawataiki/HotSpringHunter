#pragma once
#include "BackGround/ObjectBase.h"

class Fence :public ObjectBase
{
public:
	Fence();
	void Update();
	void Render(RenderContext& rc)override;

	void Init(Vector3& pos, Quaternion& rot, Vector3& sca)override {
		m_objeModel.SetTRS(pos, rot, sca);
		m_objeModel.Update();
		m_underPosition = pos;
		m_position = pos;
		m_underPosition.y -= 400.0f;
	}

	void UpFence() {
		m_moveDirection = true;
		m_isDraw = true;
		m_isLerp = true;
	}

	void DownFence() {
		m_moveDirection = false;
		m_isLerp = true;
	}


private:
	bool m_isDraw = false;						
	bool m_isLerp = false;						
	bool m_moveDirection = true;				

	float m_t = 0.0f;							//リープ補完率

	Vector3 m_underPosition = Vector3::Zero;	
	Vector3 m_position = Vector3::Zero;			
};

