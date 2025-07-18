#include "stdafx.h"
#include "NowLoading.h"

namespace
{
	// Loading画像のファイルパス
	constexpr const char* LOADING_OBJECT_NAME[] = {
		"Assets/modelData/image/nowloading_n.dds",
		"Assets/modelData/image/nowloading_o.dds",
		"Assets/modelData/image/nowloading_w.dds",
		"Assets/modelData/image/nowloading_l.dds",
		"Assets/modelData/image/nowloading_o.dds",
		"Assets/modelData/image/nowloading_a.dds",
		"Assets/modelData/image/nowloading_d.dds",
		"Assets/modelData/image/nowloading_i.dds",
		"Assets/modelData/image/nowloading_n.dds",
		"Assets/modelData/image/nowloading_g.dds",
	};
	const char* FindName(const int index)
	{
		//エラーが出ないようにする ラップする
		if (index < ARRAYSIZE(LOADING_OBJECT_NAME)) {
			return LOADING_OBJECT_NAME[index];
		}
		K2_ASSERT(false, "指定したタイプがありません。\n");
		return nullptr;
	}

	// 基準となる高さ
	constexpr float BASE_HEIGHT = -450.0f;
	// 基準となる横
	constexpr float BASE_WIDTH = 200.0f;
	// 加算する横
	constexpr float ADD_WIDTH = 60.0f;
	// 加算する高さ
	constexpr float ADD_HEIGHT = 20.0f;
	// 画像の大きさ
	const Vector2 IMAGE_SIZE = Vector2(50.0f, 50.0);
}

void LoadingObject::Initialize(const int index)
{
	const char* name = FindName(index);
	m_spriteRender.Init(name, IMAGE_SIZE.x, IMAGE_SIZE.y);
	m_position = Vector2(BASE_WIDTH + (index * ADD_WIDTH), BASE_HEIGHT);
	m_spriteRender.SetPosition(m_position);
}

void LoadingObject::Update()
{
	// サインカーブをつかって上下に動かす
	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	if (m_isAnimation) {
		m_position.y = BASE_HEIGHT + (std::sin(m_sinCurveTime) * ADD_HEIGHT);
		m_spriteRender.SetPosition(m_position);
		m_sinCurveTime += (deltaTime * 5.0f);
	}
	m_spriteRender.Update();
}

void LoadingObject::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}

void LoadingObject::PlayAnimation()
{
	m_isAnimation = true;
}




/********************************/


NowLoading::NowLoading()
{
	m_loadingObjects.clear();
}


NowLoading::~NowLoading()
{
	for (LoadingObject* object : m_loadingObjects) {
		delete object;
		object = nullptr;
	}
	m_loadingObjects.clear();
}


bool NowLoading::Start()
{
	for (int i = 0; i < ARRAYSIZE(LOADING_OBJECT_NAME); ++i) {
		LoadingObject* loadingObject = new LoadingObject();
		loadingObject->Initialize(i);
		m_loadingObjects.push_back(loadingObject);
	}
	return true;
}


void NowLoading::Update()
{
	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	//範囲for:配列１個１個を持ってくる
	for (int i = 0; i < ARRAYSIZE(LOADING_OBJECT_NAME); ++i) {
		LoadingObject* object = m_loadingObjects[i];

		// 指定した時間を超えるとアニメーション開始！
		// そのままiを使うと１秒ごとになるので小細工する
		const float targetTime = static_cast<float>(i) / 10.0f;
		if (targetTime <= m_elapsedTime) {
			object->PlayAnimation();
		}

		object->Update();
	}
	m_elapsedTime += deltaTime;
}


void NowLoading::Render(RenderContext& rc)
{
	for (auto* object : m_loadingObjects) {
		object->Render(rc);
	}
}