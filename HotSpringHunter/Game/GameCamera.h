#pragma once

namespace Character {
	class Player;
}

enum EnCameraVar {
	enFollow,			//追従カメラ
	enLookDown,			//見下ろし固定カメラ
	enBearEventCamera		//クマ登場のイベントカメラ
};

namespace Character {
	class Player;
}
class Bear;
class GameCamera :public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start()override;
	void Update()override;
	//カメラ切り替え
	void CameraSwitch();
	//追従カメラの更新
	void FollowCamera();
	//俯瞰カメラの更新
	void LookDownCamera();
	//クマ登場イベントカメラの更新
	void BearContactCamera();
	//カメラ更新
	void CameraUpdate();
	//カメラを障害物エリアモードに移行
	void SwitchObstaclesMode();
	//カメラを戦闘エリアモードに移行
	void SwitchBattleMode();
	//カメラをボスエリアモードに移行	
	void SwitchBossBattleMode();

	//セッター
	//カメラ状態を設定
	inline void SetCameraState(const EnCameraVar var) { m_cameraState = var; };

	//ゲッター
	//カメラ状態を取得
	inline int GetCameraState() const { return m_cameraState; };

private:
	Character::Player*m_player				= nullptr;
	Bear*			m_bear					= nullptr;

	Vector3			m_cameraPos				= Vector3::Zero;		//カメラ座標
	Vector3			m_cameraTarget			= Vector3::Zero;		//カメラ注視点

	float			m_eventTimeLapse		= 0.0f;					//イベントカメラの経過時間
	float			m_cameraLeftLimitPos	= 0.0f;					//カメラの左方向の限界値
	float			m_cameraRightLimitPos	= 0.0f;					//カメラの右方向の限界値

	int				m_cameraState			= 0;					//カメラの状態
};


