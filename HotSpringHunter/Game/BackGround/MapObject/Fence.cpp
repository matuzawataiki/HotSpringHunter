#include "stdafx.h"
#include "Fence.h"
#include "FenceManager.h"

Fence::Fence()
{
	m_objeModel.Init("Assets/stage/fence.tkm");
}

void Fence::Update()
{
	if(!m_isLerp){
		return;
	}

	Vector3 lerp;
	lerp.Lerp(m_t, m_underPosition, m_position);
	m_objeModel.SetPosition(lerp);
	m_objeModel.Update();
	
	if (m_moveDirection) {
		m_t += 0.01;
		if (m_t >= 1.0f) {
			m_isLerp = false;
		}
	}
	else {
		m_t -= 0.01;
		if (m_t <= 0.0f) {
			m_isLerp = false;
			m_isDraw = true;
		}
	}
	

}

void Fence::Render(RenderContext& rc)
{
	if (m_isDraw) {
		m_objeModel.Draw(rc);
	}
}
