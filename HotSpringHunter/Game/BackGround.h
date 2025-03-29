#pragma once
class BackGround :public IGameObject
{
public:
	BackGround();
	~BackGround();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	//backGround座標のゲッター。
	Vector3 GetStagePosition() {
		return m_position;
	}
private:
	Vector3 m_position = Vector3::Zero;
	ModelRender m_modelRender;
	PhysicsStaticObject m_StageCollision;
};