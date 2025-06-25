#pragma once

enum enSmokeType
{
	enSmokeType1,
	enSmokeType2,
	enSmokeType3,
	enSmokeTypeMax
};

class Smoke:public IGameObject
{
public:
	Smoke();
	~Smoke();

	bool Start() override;
	void Update() override;
	void Render(RenderContext&rc) override;

	void SetSmokeType(int type) 
	{ 
		m_smokeType = type;
	}
	void SetSmokePos(const Vector2& smokePos) //変えない値(const)と参照渡し(~~&)
	{
		m_smokePos = smokePos;
	}
	const Vector2& GetSmokePos() const //ここの値は絶対に変えない const ~~ const
	{
		return m_smokePos;
	}
	
private:
	SpriteRender m_smokeModel;
	Vector2      m_smokePos;

	int m_smokeType;
};

