#pragma once
namespace Character{
	class Player;
}
class PowerUpBox :public IGameObject
{
public:
	PowerUpBox();
	~PowerUpBox();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	enum EnPowerUp
	{
		enSlash,
		enSlash1,
		enSlash2,
		enSlash3,
		enRangeAttack,
		enRangeAttack1,
		enRangeAttack2,
		enRangeAttack3,
		enNum,
	};

	void InitBox(EnPowerUp powerUp) {
		m_num = powerUp;
	}

private:
	Character::Player* m_plaeyr = nullptr;

	ModelRender m_boxModel;
	Vector3 m_position = Vector3::Zero;
	EnPowerUp m_num;

	float m_time = 0.0f;

	bool m_isMainPowerUp = true;
	bool m_isSubPowerUp = true;
};

