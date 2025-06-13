#include "stdafx.h"
#include "Smoke.h"

namespace 
{
	//湯気3種類
	const char* FILE_PASS[] = {
		"Assets/modelData/UI/yuge/yuge1.dds",
		"Assets/modelData/UI/yuge/yuge2.dds",
		"Assets/modelData/UI/yuge/yuge3.dds",
	};
}

Smoke::Smoke()
{
}

Smoke::~Smoke()
{
}

bool Smoke::Start()
{
	//湯気の作成
	m_smokeModel.Init(FILE_PASS[m_smokeType],1200.0f,1000.0f);
	m_smokeModel.SetPosition(m_smokePos);
	m_smokeModel.Update();
	return true;
}
void Smoke::Update()
{
	m_smokeModel.SetPosition(m_smokePos);
	m_smokeModel.Update();
}

void Smoke::Render(RenderContext& rc)
{
	m_smokeModel.Draw(rc);
}