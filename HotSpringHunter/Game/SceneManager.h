#pragma once

namespace Character {
	class Player;
}

enum EnGameScene {
	enStartArea,		//ゲーム開始
	enBattleArea1Start,	//戦闘エリア1
	enBattleArea1,		//戦闘エリア1
	enBattleArea1Clear,	//戦闘エリア1クリア
	enBattleArea2Start,	//戦闘エリア2
	enBattleArea2,		//戦闘エリア2
	enBattleArea2Clear,	//戦闘エリア2クリア
	enBossAreaStart,	//ボスエリア
	enBossArea,			//ボスエリア
	enDefeatedBoss,		//ボスを倒した
	enGoalArea,			//ゴール
};

class FenceManager;
class EnemySpawner;
class GameClear;
class GameOver;
class GameCamera;
class Bear; 
class Result;
class StageManager;
class SoundEffect;

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
	//倒した敵の数を追加
	void AddDefeatedEnemyCount() { m_defeatedEnemyes++; };
	//ゲームタイム
	void GameTimeUpdate();
	//
	void Render(RenderContext& rc)
	{
		m_timeRender.Draw(rc);
	}

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

	float m_gamePlayTime = 0.0f;

private:
	EnemySpawner*		m_enemySpawner	= nullptr;
	GameClear*			m_gameClear		= nullptr;
	GameOver*           m_gameOver      = nullptr;
	GameCamera*			m_gameCamera	= nullptr;
	Character::Player*	m_player		= nullptr;
	Bear*				m_bear			= nullptr;
	Result*             m_result        = nullptr;
	StageManager*		m_stageManager  = nullptr;
	SoundEffect*        m_soundEffect   = nullptr;

	EnGameScene			m_sceneState = EnGameScene::enStartArea;	//シーン状態

	FontRender          m_timeRender;

	float				m_gameClearTime = 0.0f;		//ゲームのプレイ時間
	
	int					m_finalScore	= 0;			//最終スコア
	int					m_defeatedEnemyes = 0;			//倒した敵の数

	bool				m_isAddMoreEnemy = false;		//エネミーを追加したか
	bool				m_isToGoal		= false;		//ゴールに移動させるかのフラッグ
	bool				m_isClearFrag	= false;		//クリア演出を実行するか
	bool                m_isTimeOff     = true;        //タイムオフ
};