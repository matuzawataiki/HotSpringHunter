#include "stdafx.h"
#include "WildBoar.h"

WildBoar::WildBoar()
{
}

WildBoar::~WildBoar()
{
}

bool WildBoar::Start()
{

	return true;
}

void WildBoar::Update()
{

}

void WildBoar::ManageState()
{

}

void WildBoar::Render(RenderContext& rc)
{
	m_wildBoarModel.Draw(rc);
}
