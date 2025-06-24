#pragma once
#include "BackGround/StageObject.h"

class Stage : public IGameObject
{
public:
	Stage();
	~Stage();
	bool Start()override;
	void Update()override;

	/// <summary>
	/// ステージの種類
	/// </summary>
	enum EnStageType
	{
		enStart,
		enBattle,
		enBoss,
		enGoal,
		enNum
	};

	/// <summary>
	/// ステージの初期化
	/// </summary>
	/// <param name="stageNum">ステージの種類</param>
	/// <param name="objData">レベルオブジェクト</param>
	void Init(EnStageType stageNum, LevelObjectData& objData);

	/// <summary>
	/// 地面のタイプの設定
	/// </summary>
	void InitStage();

	/// <summary>
	/// オブジェクトの設置や登録
	/// </summary>
	void InitLevel();

	/// <summary>
	/// 地面の設置
	/// </summary>
	/// <param name="objData">地面の種類</param>
	/// <param name="position">レベルオブジェクト</param>
	void NewGround(LevelObjectData& objData, Vector3& position);

	/// <summary>
	/// 当たり判定の設置
	/// </summary>
	/// <param name="objData">当たり判定種類</param>
	/// <param name="position">レベルオブジェクト</param>
	void NewHitBox(LevelObjectData& objData, Vector3& position);

	/// <summary>
	/// フェンスを出す
	/// </summary>
	void UpFence() {
		m_stageObject.fence[0]->UpFence();
		m_stageObject.fence[1]->UpFence();
		m_stageObject.hitBoxFence[0]->CreatePhysicsObject();
		m_stageObject.hitBoxFence[1]->CreatePhysicsObject();
	}

	/// <summary>
	/// 前方のフェンスを消す
	/// </summary>
	void DeleteFence() {
		m_stageObject.fence[1]->DownFence();
		DeleteGO(m_stageObject.hitBoxFence[1]);
	}

	/// <summary>
	/// ステージオブジェクトを取得
	/// </summary>
	/// <returns></returns>
	StageObject& GetStageObject() {
		return m_stageObject;
	}

private:
	LevelRender m_stageLevel;

	StageObject m_stageObject;

	Vector3 m_position = Vector3::Zero;

	bool isInit = false;

	EnStageType m_stageNum = enNum;
	const char* m_levelFilePath = nullptr;
	const wchar_t* m_grondName = nullptr;
	const wchar_t* m_hitBoxName = nullptr;

};

