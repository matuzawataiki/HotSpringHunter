#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "BackGround.h"

namespace {
	const float CAMERA_NEAR = 1.0f;
	const float CAMERA_FAR = 10000.0f;
};

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");
	m_backGround = FindGO<BackGround>("backGround");

	m_cameraState = EnCameraVar::lookDown;
	//近平面
	g_camera3D->SetNear(CAMERA_NEAR);
	//遠平面
	g_camera3D->SetFar(CAMERA_FAR);

	return true;
}

void GameCamera::Update()
{
	CameraSwitch();
	CameraUpdate();
}

/// <summary>
/// カメラ切り替え。
/// </summary>
void GameCamera::CameraSwitch()
{
	////Bボタンでカメラ切り替え(仮）。
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	if (m_cameraState == EnCameraVar::follow) {
	//		m_cameraState = EnCameraVar::lookDown;
	//	}
	//	else if (m_cameraState == EnCameraVar::lookDown) {
	//		m_cameraState = EnCameraVar::follow;
	//	}
	//}

	//カメラのstate管理。
	//俯瞰カメラ。
	if (m_cameraState == EnCameraVar::lookDown) {
		m_followPos = m_newFollowPos;
		LookDownCamera();
	}
	//追従カメラ。
	else if (m_cameraState == EnCameraVar::follow) {
		FollowCamera();
	}
}

/// <summary>
/// 追従カメラ。
/// </summary>
void GameCamera::FollowCamera()
{
	//注視点をプレイヤーの座標に設定。
	m_target = m_player->GetPlayerPos();

	//カメラ座標を設定。
	m_toCameraPos.Set(m_followPos);
}

/// <summary>
/// 見下ろしカメラ。
/// </summary>
void GameCamera::LookDownCamera()
{
	//注視点を該当戦闘エリアの中心に設定(一旦ステージの中心）。
	m_target = m_player->GetPlayerPos();

	//カメラ座標を設定。
	m_toCameraPos.Set(m_lookDownPos);
}

/// <summary>
/// カメラ更新。
/// </summary>
void GameCamera::CameraUpdate()
{
	//視点を計算。
	Vector3 pos = m_target + m_toCameraPos;

	//カメラに注視点と視点を設定。
	g_camera3D->SetTarget(m_target);
	g_camera3D->SetPosition(pos);

	//カメラ更新。
	g_camera3D->Update();
}