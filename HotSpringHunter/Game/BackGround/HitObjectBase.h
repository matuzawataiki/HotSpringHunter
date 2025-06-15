#pragma once
class HitObjectBase : public IGameObject
{
public:
	HitObjectBase() {m_physicsStaticObject = new PhysicsStaticObject;}
	~HitObjectBase() { delete m_physicsStaticObject; }

	void Init(Vector3& pos, Quaternion& rot, Vector3& sca)
	{
		m_objeModel.SetTRS(pos, rot, sca);
		m_objeModel.Update();
	}

	void CreatePhysicsObject();

protected:
	ModelRender m_objeModel;
	PhysicsStaticObject* m_physicsStaticObject;
};

