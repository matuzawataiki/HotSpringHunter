#include "stdafx.h"
#include "StageManager.h";

#include "BackGround/MapObject/BushBig.h";
#include "BackGround/MapObject/BushSmall.h";
#include "BackGround/MapObject/Tree.h";

#include "BackGround/MapObject/GroundStart.h";
#include "BackGround/MapObject/GroundRun.h";
#include "BackGround/MapObject/GroundBattle.h";
#include "BackGround/MapObject/GroundBossBattle.h";
#include "BackGround/MapObject/GroundGool.h";

#include "BackGround/HitObject/HitBoxStart.h";
#include "BackGround/HitObject/HitBoxRun.h";
#include "BackGround/HitObject/HitBoxBattle.h";
#include "BackGround/HitObject/HitBoxBossBattle.h";
#include "BackGround/HitObject/HitBoxGool.h";

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

bool StageManager::Start()
{
	//スタート
	m_stageLevel_1.Init("Assets/stage/LevelStart.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}
		
		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundStart") == true) {
			auto groundStart = NewGO<GroundStart>(0, "groundStart");
			groundStart->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"StartHitBox") == true) {
			auto hitBoxStart = NewGO<HitBoxStart>(0, "hitBoxStart");
			hitBoxStart->Init(objData.position, objData.rotation, objData.scale);
			hitBoxStart->CreatePhysicsObject();
			return true;
		}
		
		return false;
		});

	//障害物エリア
	m_RunLevel_1.Init("Assets/stage/LevelRun1.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundRun") == true) {
			auto groundRun = NewGO<GroundRun>(0, "groundRun");
			groundRun->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"RunHitBox") == true) {
			auto hitBoxRun = NewGO<HitBoxRun>(0, "hitBoxRun");
			hitBoxRun->Init(objData.position, objData.rotation, objData.scale);
			hitBoxRun->CreatePhysicsObject();
			return true;
		}

		return false;
		});

	//戦闘エリア
	m_BattleLevel_1.Init("Assets/stage/LevelBattle1.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundBattle") == true) {
			auto groundBattle = NewGO<GroundBattle>(0, "groundBattle");
			groundBattle->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"BattleHitBox") == true) {
			auto hitBoxBattle = NewGO<HitBoxBattle>(0, "hitBoxBattle");
			hitBoxBattle->Init(objData.position, objData.rotation, objData.scale);
			hitBoxBattle->CreatePhysicsObject();
			return true;
		}

		return false;
		});

	//障害物エリア
	m_RunLevel_2.Init("Assets/stage/LevelRun2.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundRun") == true) {
			auto groundRun = NewGO<GroundRun>(0, "groundRun");
			groundRun->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"RunHitBox") == true) {
			auto hitBoxRun = NewGO<HitBoxRun>(0, "hitBoxRun");
			hitBoxRun->Init(objData.position, objData.rotation, objData.scale);
			hitBoxRun->CreatePhysicsObject();
			return true;
		}

		return false;
		});


	//ボスエリア
	m_BossBattleLevel_1.Init("Assets/stage/LevelBossBattle1.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundBossBattle") == true) {
			auto groundBossBattle = NewGO<GroundBossBattle>(0, "groundBossBattle");
			groundBossBattle->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"BossBattleHitBox") == true) {
			auto hitBoxBossBattle = NewGO<HitBoxBossBattle>(0, "hitBoxBossBattle");
			hitBoxBossBattle->Init(objData.position, objData.rotation, objData.scale);
			hitBoxBossBattle->CreatePhysicsObject();
			return true;
		}

		return false;
		});

	m_GoolLevel_1.Init("Assets/stage/LevelGool1.tkl", [&](LevelObjectData& objData) {
		//大型ブッシュの配置
		if (objData.EqualObjectName(L"bush1") == true) {
			auto bush1 = NewGO<BushBig>(0, "bush1");
			bush1->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//小型ブッシュの配置
		if (objData.EqualObjectName(L"bush2") == true) {
			auto bush2 = NewGO<BushSmall>(0, "bush2");
			bush2->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//木の配置
		if (objData.EqualObjectName(L"tree") == true) {
			auto tree = NewGO<Tree>(0, "tree");
			tree->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//地面の配置
		if (objData.EqualObjectName(L"GroundGool") == true) {
			auto groundGool = NewGO<GroundGool>(0, "groundGool");
			groundGool->Init(objData.position, objData.rotation, objData.scale);
			return true;
		}

		//当たり判定の設置
		if (objData.EqualObjectName(L"GoolHitBox") == true) {
			auto hitBoxGool = NewGO<HitBoxGool>(0, "hitBoxGool");
			hitBoxGool->Init(objData.position, objData.rotation, objData.scale);
			hitBoxGool->CreatePhysicsObject();
			return true;
		}

		return false;
		});
	return true;
}
