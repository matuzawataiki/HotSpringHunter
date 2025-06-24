#pragma once

class Smoke;

class SmokeEmitter:public IGameObject
{
public:
	SmokeEmitter();
	~SmokeEmitter();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
	void DeactiveEmitter()
	{
		m_isActiveEmitter = false;
	}

private:
	std::vector<Smoke*>m_smokeList;

	float m_mekeSmokeElapsedTime = 0.0f;

	int m_positionIndex = 0;

	bool m_isActiveEmitter = true;
};

