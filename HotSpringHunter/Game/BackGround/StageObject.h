#pragma once
#include "stdafx.h"

#include "BackGround/HitObject/HitBoxBattle.h"
#include "BackGround/HitObject/HitBoxBossBattle.h"
#include "BackGround/HitObject/HItBoxFence.h"
#include "BackGround/HitObject/HitBoxGool.h"
#include "BackGround/HitObject/HitBoxStart.h"
#include "BackGround/HitObject/InOutHitBox.h"

#include "BackGround/MapObject/BushBig.h"
#include "BackGround/MapObject/BushSmall.h"
#include "BackGround/MapObject/Fence.h"
#include "BackGround/MapObject/GroundBattle.h"
#include "BackGround/MapObject/GroundBossBattle.h"
#include "BackGround/MapObject/GroundGool.h"
#include "BackGround/MapObject/GroundStart.h"
#include "BackGround/MapObject/HotSpring.h"
#include "BackGround/MapObject/Tree.h"

struct StageObject
{
	HitBoxBattle* hitBoxBattle = nullptr;
	HitBoxBossBattle* hitBoxBossBattle = nullptr;
	HitBoxGoal* hitBoxGoal = nullptr;
	HitBoxStart* hitBoxStart = nullptr;
	InOutHitBox* inOutHitBox = nullptr;

	GroundBattle* grondBattle = nullptr;
	GroundBossBattle* grondBossBattle = nullptr;
	GroundGool* grondGoal = nullptr;
	GroundStart* grondStart = nullptr;
	HotSpring* hotSpring = nullptr;

	std::vector<BushBig*> bushBig;
	std::vector<BushSmall*> bushSmall;
	std::vector<Tree*> tree;

	HItBoxFence* hitBoxFence[2] = { nullptr,nullptr };
	Fence* fence[2] = { nullptr,nullptr };

	Vector3 enemyPos[20];
	Vector3 enemyTargetPos[20];
	Vector3 bossPos;
};