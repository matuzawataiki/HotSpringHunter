#pragma once

namespace Character {
	class Player;
}

enum EnGameScene {
	enStart,				//ゲーム開始
	enObstacleArea1,		//障害物エリア1
	enBattleArea1,			//戦闘エリア1
	enObstacleArea2,		//障害物エリア2
	enBattleArea2,			//戦闘エリア2
	enDefeatedBoss,			//ボスを倒した
	enGoal,					//ゴール
};

class FenceManager;
class EnemySpawner;
class GameClear;
class GameCamera;
class Bear; 

class SceneManager:public IGameObject
{
public:
	SceneManager();
	~SceneManager();
	bool Start()override;
	void Update()override;
	//インゲーム中のゲームシーンを管理
	void InGameSceneManage();
	//シーン切り替わりの処理
	void SwitchingScenes();

	//セッター
	//クリア演出フラッグを設定
	inline void SetIsClearFrag(const bool isClear) { m_isClearFrag = isClear; };

	//ゲッター
	//シーン状態を取得
	inline int GetGameScene() const { return m_sceneState; };
	//ゴール移動のフラッグを取得
	inline bool GetIsToGoal() const { return m_isToGoal; };
	//プレイ時間を取得
	inline float GetInGameTime() const { return m_gameClearTime; };
	//最終スコアを取得
	inline int GetFinalScore() const { return m_finalScore; };


private:
	FenceManager*		m_fenceManager	= nullptr;
	EnemySpawner*		m_enemySpawner	= nullptr;
	GameClear*			m_gameClear		= nullptr;
	GameCamera*			m_gameCamera	= nullptr;
	Character::Player*	m_player		= nullptr;
	Bear*				m_bear			= nullptr;

	float				 m_gameClearTime = 0.0f;		//ゲームのプレイ時間

	int					m_sceneState	= 0;			//シーン状態
	int					m_finalScore	= 0;			//最終スコア

	bool				m_isExecuted	= false;		//シーン切り替え時の処理を実行したか
	bool				m_isToGoal		= false;		//ゴールに移動させるかのフラッグ
	bool				m_isClearFrag	= false;		//クリア演出を実行するか
};