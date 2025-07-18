#pragma once
class BreakedRock : public IGameObject
{
public:
	static const int MAX_CREATE_NUM = 5;

public:
	BreakedRock();
	~BreakedRock();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//セットポジション　外から設定できる
	void SetPosition(const Vector3 &position)
	{
		m_position = position;
	}

private:
	ModelRender m_breakedRock;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_velocity = Vector3::Front;
};

