#include "stdafx.h"
#include "Stage.h";

#include "BackGround/MapObject/BushBig.h"
#include "BackGround/MapObject/BushSmall.h"
#include "BackGround/MapObject/Tree.h"
#include "BackGround/MapObject/Fence.h"
#include "BackGround/MapObject/HotSpring.h"

#include "BackGround/MapObject/GroundStart.h"
#include "BackGround/MapObject/GroundBattle.h"
#include "BackGround/MapObject/GroundBossBattle.h"
#include "BackGround/MapObject/GroundGool.h"

#include "BackGround/HitObject/HitBoxStart.h"
#include "BackGround/HitObject/HitBoxBattle.h"
#include "BackGround/HitObject/HitBoxBossBattle.h"
#include "BackGround/HitObject/HitBoxGool.h"
#include "BackGround/HitObject/HitBoxFence.h"
#include "BackGround/HitObject/InOutHitBox.h"

Stage::Stage()
{
}

Stage::~Stage()
{
	for (auto bushB : m_stageObject.bushBig) {
		DeleteGO(bushB);
	}
	m_stageObject.bushBig.clear();

	for (auto bushS : m_stageObject.bushSmall) {
		DeleteGO(bushS);
	}
	m_stageObject.bushSmall.clear();

	for (auto tree : m_stageObject.tree) {
		DeleteGO(tree);
	}
	m_stageObject.tree.clear();

	switch (m_stageNum)
	{
	case enStart:
		DeleteGO(m_stageObject.grondStart);
		DeleteGO(m_stageObject.hitBoxStart);
		break;

	case enBattle:
		DeleteGO(m_stageObject.grondBattle);
		DeleteGO(m_stageObject.grondBossBattle);
		DeleteGO(m_stageObject.fence[0]);
		DeleteGO(m_stageObject.fence[1]);
		DeleteGO(m_stageObject.hitBoxFence[0]);
		DeleteGO(m_stageObject.hitBoxFence[1]);
		break;

	case enBoss:
		DeleteGO(m_stageObject.grondBossBattle);
		DeleteGO(m_stageObject.hitBoxBossBattle);
		DeleteGO(m_stageObject.fence[0]);
		DeleteGO(m_stageObject.fence[1]);
		DeleteGO(m_stageObject.hitBoxFence[0]);
		DeleteGO(m_stageObject.hitBoxFence[1]);
		break;

	case enGoal:
		DeleteGO(m_stageObject.grondGoal);
		DeleteGO(m_stageObject.hitBoxGoal);
		DeleteGO(m_stageObject.hotSpring);
		break;
	default:
		break;
	}
}

bool Stage::Start()
{
	return true;
}

void Stage::Update()
{
}

void Stage::Init(EnStageType stageNum, LevelObjectData& objData)
{
	m_stageNum = stageNum;
	m_position = objData.position;
	InitStage();
	InitLevel();
}

void Stage::InitLevel()
{
	int countStart = 0;
	int countEnd = 0;
	//スタート
	m_stageLevel.Init(m_levelFilePath, [&](LevelObjectData& objData) {
		Vector3 position = m_position + objData.position;
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(position, objData.rotation, objData.scale);
			m_stageObject.bushBig.push_back(bush1);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(position, objData.rotation, objData.scale);
			m_stageObject.bushSmall.push_back(bush2);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(position, objData.rotation, objData.scale);
			m_stageObject.tree.push_back(tree);
			return true;
		}

		if (objData.EqualObjectName(L"enemyEnd") == true) {
			m_stageObject.enemyTargetPos[countEnd] = position;
			countEnd++;
			return true;
		}

		if (objData.EqualObjectName(L"enemyStart") == true) {
			m_stageObject.enemyPos[countStart] = position;
			countStart++;
			return true;
		}

		if (objData.EqualObjectName(L"BossPosition") == true) {
			m_stageObject.bossPos = position;
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(m_grondName) == true) {
			NewGround(objData, position);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(m_hitBoxName) == true) {
			NewHitBox(objData, position);
			return true;
		}

		if (objData.EqualObjectName(L"InOutBoxBattle") == true) {
			m_stageObject.inOutHitBox = NewGO<InOutHitBox>(0, "inOutBoxBattle");
			m_stageObject.inOutHitBox->init(position, objData.rotation, objData.scale);
			return true;
		}

		if (objData.EqualObjectName(L"BackFence") == true) {
			//m_stageObject.fence[0] = NewGO<Fence>(0, "fence");
			//m_stageObject.fence[0]->Init(position, objData.rotation, objData.scale);
			return true;
		}

		if (objData.EqualObjectName(L"FrontFence") == true) {
			//m_stageObject.fence[1] = NewGO<Fence>(0, "fence");
			//m_stageObject.fence[1]->Init(m_position, objData.rotation, objData.scale);
			return true;
		}

		if (objData.EqualObjectName(L"BackHitBoxFence") == true) {
			//m_stageObject.hitBoxFence[0] = NewGO<HItBoxFence>(0, "hitBoxFence");
			//m_stageObject.hitBoxFence[0]->Init(m_position, objData.rotation, objData.scale);
			return true;
		}

		if (objData.EqualObjectName(L"FrontHitBoxFence") == true) {
			//m_stageObject.hitBoxFence[1] = NewGO<HItBoxFence>(0, "hitBoxFence");
			//m_stageObject.hitBoxFence[1]->Init(m_position, objData.rotation, objData.scale);
			return true;
		}

		if (objData.EqualObjectName(L"HotSpring") == true) {
			m_stageObject.hotSpring = NewGO<HotSpring>(0, "hotSpring");
			m_stageObject.hotSpring->Init(m_position, objData.rotation, objData.scale);
			return true;
		}

		return false;
		});
}

void Stage::InitStage()
{
	switch (m_stageNum)
	{
	case enStart:
		m_levelFilePath = "Assets/stage/Level/LevelStart.tkl";
		m_grondName = L"GroundStart";
		m_hitBoxName = L"HitBoxStart";
		break;

	case enBattle:
		m_levelFilePath = "Assets/stage/Level/LevelBattle.tkl";
		m_grondName = L"GroundBattle";
		m_hitBoxName = L"HitBoxBattle";
		break;

	case enBoss:
		m_levelFilePath = "Assets/stage/Level/LevelBoss.tkl";
		m_grondName = L"GroundBoss";
		m_hitBoxName = L"HitBoxBoss";
		break;

	case enGoal:
		m_levelFilePath = "Assets/stage/Level/LevelGoal.tkl";
		m_grondName = L"GroundGoal";
		m_hitBoxName = L"HitBoxGoal";
		break;
	default:
		break;
	}
}

void Stage::NewGround(LevelObjectData& objData, Vector3& position)
{
	switch (m_stageNum)
	{
	case enStart:
		m_stageObject.grondStart = NewGO<GroundStart>(0, "groundStart");
		m_stageObject.grondStart->Init(position, objData.rotation, objData.scale);
		break;

	case enBattle:
		m_stageObject.grondBattle = NewGO<GroundBattle>(0, "groundBattle");
		m_stageObject.grondBattle->Init(position, objData.rotation, objData.scale);
		break;

	case enBoss:
		m_stageObject.grondBossBattle = NewGO<GroundBossBattle>(0, "groundBoss");
		m_stageObject.grondBossBattle->Init(position, objData.rotation, objData.scale);
		break;

	case enGoal:
		m_stageObject.grondGoal = NewGO<GroundGool>(0, "groundGoal");
		m_stageObject.grondGoal->Init(position, objData.rotation, objData.scale);
		break;
	default:
		break;
	}
}

void Stage::NewHitBox(LevelObjectData& objData, Vector3& position)
{
	switch (m_stageNum)
	{
	case enStart:
		m_stageObject.hitBoxStart = NewGO<HitBoxStart>(0, "hitBoxStart");
		m_stageObject.hitBoxStart->Init(position, objData.rotation, objData.scale);
		m_stageObject.hitBoxStart->CreatePhysicsObject();
		break;

	case enBattle:
		m_stageObject.hitBoxBattle = NewGO<HitBoxBattle>(0, "hitBoxBattle");
		m_stageObject.hitBoxBattle->Init(position, objData.rotation, objData.scale);
		m_stageObject.hitBoxBattle->CreatePhysicsObject();
		break;

	case enBoss:
		m_stageObject.hitBoxBossBattle = NewGO<HitBoxBossBattle>(0, "hitBoxBoss");
		m_stageObject.hitBoxBossBattle->Init(position, objData.rotation, objData.scale);
		m_stageObject.hitBoxBossBattle->CreatePhysicsObject();
		break;

	case enGoal:
		m_stageObject.hitBoxGoal = NewGO<HitBoxGoal>(0, "hitBoxGoal");
		m_stageObject.hitBoxGoal->Init(position, objData.rotation, objData.scale);
		m_stageObject.hitBoxGoal->CreatePhysicsObject();
		break;
	default:
		break;
	}
}
