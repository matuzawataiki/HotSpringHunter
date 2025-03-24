#pragma once

class Player;
class BackGround;
class GameCamera :public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start()override;
	void Update()override;
	//追従カメラ。
	void FollowCamera();
	//俯瞰カメラ。
	void LookDownCamera();
	//カメラ切り替え。
	void CameraSwitch();
	//カメラ更新。
	void CameraUpdate();

	Vector3 GetPlaneToCameraPos() {
		return m_toCameraPos;
	}
private:
	Player* m_player = nullptr;
	BackGround* m_backGround = nullptr;

	Vector3 m_toCameraPos = Vector3::Zero;					//カメラ座標。
	Vector3 m_toCameraPosOld = Vector3::Zero;				//変更前カメラ座標。
	Vector3 m_target = Vector3::Zero;						//カメラ注視点。
	Vector3 m_followPos = Vector3{ 0.0f,250.0f,-400.0f };	//追従カメラ座標。
	Vector3 m_newFollowPos = Vector3{ 0.0f,250.0f,-400.0f };//追従カメラ初期値。
	Vector3 m_lookDownPos = Vector3{ 0.0f,750.0f,-300.0f };	//俯瞰カメラ座標。


	int m_cameraState = 0;				//カメラの状態。
	enum EnCameraVar {
		follow,
		lookDown,
	};
};


