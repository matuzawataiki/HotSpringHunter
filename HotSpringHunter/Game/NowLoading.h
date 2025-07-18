#pragma once

/// <summary>
/// ロード中の文字をアニメーションさせたりするクラスです
/// </summary>
class LoadingObject
{
public:
	LoadingObject() {}
	~LoadingObject() {}

	void Initialize(const int index);
	void Update();
	void Render(RenderContext& rc);

	void PlayAnimation();

private:
	bool m_isAnimation = false;
	float m_sinCurveTime = 0.0f;
	Vector2 m_position = Vector2::Zero;

	SpriteRender m_spriteRender;
};


class NowLoading : public IGameObject
{
public:
	NowLoading();
	~NowLoading();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	float m_elapsedTime = 0.0f;
	std::vector<LoadingObject*> m_loadingObjects;
};

