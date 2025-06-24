#pragma once
class ObjectBase: public IGameObject
{
public:
	ObjectBase();
	~ObjectBase();

	virtual void Init(Vector3& pos, Quaternion& rot, Vector3& sca)
	{
		m_objeModel.SetTRS(pos, rot, sca);
		m_objeModel.Update();
	}

	virtual void Render(RenderContext& rc);

protected:
	nsTMEngine::ModelRender m_objeModel;

};

