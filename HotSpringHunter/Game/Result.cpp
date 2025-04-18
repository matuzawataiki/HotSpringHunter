#include "stdafx.h"
#include "Result.h"
#include "Title.h"

Result::Result()
{
}

Result::~Result()
{

}

bool Result::Start()
{
	m_resultModel.Init("Assets/modelData/image/result.dds", 1920.0f, 1080.0f);

	return true;
}

void Result::Update()
{
	SwitchTitle();
}

void Result::SwitchTitle()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void Result::Render(RenderContext& rc)
{
	m_resultModel.Draw(rc);
}