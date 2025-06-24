#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bear.h"
#include "BackGround.h"

namespace {
	const float		CAMERA_NEAR					= 1.0f;						//カメラ近平面
	const float		CAMERA_FAR					= 30000.0f;					//カメラ遠平面
	const float		CAMERA_LEFT_LIMIT_OBSTAC	= -1250.0f;					//障害物：左方向の限界値
	const float		CAMERA_RIGHT_LIMIT_OBSTAC	= 1250.0f;					//障害物：右方向の限界値
	const float		CAMERA_LEFT_LIMIT_BATTLE	= -1200.0f;					//戦闘：左方向の限界値
	const float		CAMERA_RIGHT_LIMIT_BATTLE	= 1200.0f;					//戦闘：右方向の限界値

	const float		CAMERA_TARGET_HEIGHT		= 100.0f;					//追従カメラ：注視点を高くする量
	const Vector3	FOLLOW_CAMERA_POS			= { 0.0f,175.0f,-300.0f };	//追従カメラ：カメラ座標

	const float		CONTACT_EVENT_TIME			= 4.0f;						//クマ接触イベントカメラ：時間
	const Vector3	CONTACT_BASE_CAMERA_POS		= { 0.0f,200.0f,-400.0f };	//クマ接触イベントカメラ：基点のカメラ座標
	const float		AMOUNT_MOVE_XPOS			= 200.0f;					//クマ接触イベントカメラ：x座標を動かす量

	/// <summary>
	/// 渡された値を設定した最小、最大の範囲内に設定して返す
	/// </summary>
	/// <param name="value">設定する値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>設定後の値</returns>
	inline float Clamp(float value, const float min, const float max)
	{
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}

		return value;
	}
};

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	m_cameraState = EnCameraVar::enFollow;
	//近平面
	g_camera3D->SetNear(CAMERA_NEAR);
	//遠平面
	g_camera3D->SetFar(CAMERA_FAR);
	//障害物エリアモードに
	SwitchObstaclesMode();

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

	//カメラのステート管理
	switch (m_cameraState) {

		//俯瞰カメラ。
	case EnCameraVar::enLookDown:
		LookDownCamera();
		break;

		//追従カメラ。
	case EnCameraVar::enFollow:
		FollowCamera();
		break;

		//クマ接触のイベントカメラ
	case EnCameraVar::enBearContact:
		BearContactCamera();
		break;

	default:
		break;
	}
}

/// <summary>
/// 追従カメラ
/// </summary>
void GameCamera::FollowCamera()
{
	m_player = FindGO<Character::Player>("player");

	//注視点をプレイヤーの座標の少し上に設定
	m_cameraTarget = m_player->GetPlayerPos();
	m_cameraTarget.y += CAMERA_TARGET_HEIGHT;

	//カメラ座標を設定
	m_cameraPos = m_cameraTarget;
	m_cameraPos += FOLLOW_CAMERA_POS;
}

/// <summary>
/// 見下ろしカメラ。
/// </summary>
void GameCamera::LookDownCamera()
{
	m_backGround = FindGO<BackGround>("backGround");

	//注視点を該当戦闘エリアの中心に設定(一旦ステージの中心）。
	m_cameraTarget = m_player->GetPlayerPos();
	m_cameraTarget.y += 100.0f;

	//カメラ座標を設定
	//m_cameraPos.Set(m_lookDownPos);
}

/// <summary>
/// クマ登場イベントカメラの更新
/// </summary>
void GameCamera::BearContactCamera()
{
	m_bear = FindGO<Bear>("bear");

	//イベント経過時間を計算
	m_eventTimeLapse += g_gameTime->GetFrameDeltaTime();

	//注視点を設定
	m_cameraTarget = m_bear->GetBearPos();
	m_cameraTarget.y += CAMERA_TARGET_HEIGHT;

	//カメラ座標を設定
	m_cameraPos = m_bear->GetBearPos();
	m_cameraPos += CONTACT_BASE_CAMERA_POS;
	//x座標をイージングで動かす
	m_cameraPos.x -= AMOUNT_MOVE_XPOS / 2.0f;
	float elapseRate = m_eventTimeLapse / CONTACT_EVENT_TIME;
	float nowXPos = AMOUNT_MOVE_XPOS * elapseRate;
	m_cameraPos.x += nowXPos;

	//イベントカメラの時間が終了したら
	if (m_eventTimeLapse >= CONTACT_EVENT_TIME) {
		//時間リセット
		m_eventTimeLapse = 0.0f;
		//追従カメラに
		m_cameraState = EnCameraVar::enFollow;
	}
}

/// <summary>
/// 障害物モードを切り替えます。
/// </summary>
void GameCamera::SwitchObstaclesMode()
{
	m_cameraLeftLimitPos = CAMERA_LEFT_LIMIT_OBSTAC;
	m_cameraRightLimitPos = CAMERA_RIGHT_LIMIT_OBSTAC;
}

/// <summary>
/// バトルモードへのカメラ切り替えを行います。
/// </summary>
void GameCamera::SwitchBattleMode()
{
	m_cameraLeftLimitPos = CAMERA_LEFT_LIMIT_BATTLE;
	m_cameraRightLimitPos = CAMERA_RIGHT_LIMIT_BATTLE;
}

/// <summary>
/// カメラ更新
/// </summary>
void GameCamera::CameraUpdate()
{
	//カメラがステージの端に寄りすぎないようにする
	m_cameraPos.x = Clamp(m_cameraPos.x, m_cameraLeftLimitPos, m_cameraRightLimitPos);
	m_cameraTarget.x = Clamp(m_cameraTarget.x, m_cameraLeftLimitPos, m_cameraRightLimitPos);


	//カメラに視点と注視点を設定
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);

	//カメラ更新
	g_camera3D->Update();
}