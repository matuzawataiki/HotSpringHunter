#pragma once
#include "BackGround/ObjectBase.h";

class HotSpring : public ObjectBase
{
public:
	HotSpring() { m_objeModel.Init("Assets/stage/HotSpring.tkm"); }
	~HotSpring(){}
};

