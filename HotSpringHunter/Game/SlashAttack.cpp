#include "stdafx.h"
#include "SlashAttack.h"

#include "Player.h"
#include "collision/CollisionObject.h"
#include "Game.h"
#include "ProjectileManager.h"

namespace {
	static const float SPHERE_COLLISION_SIZE = 18.0f;
	static const float MOVE_SPEED_OFFSET = 80.0f;
	static const float DELETE_TIME = 4.0f;
	static const float DAMAGE = 20.0F;
}

SlashAttack::SlashAttack()
{

}

SlashAttack::~SlashAttack()
{
}

bool SlashAttack::Start()
{
	m_player = FindGO<Character::Player>("player");
	m_projectileManager = FindGO<ProjectileManager>("projectileManager");

	m_position = m_player->GetPlayerPos();
	m_sphereModel.Init("Assets/modelData/player/slash.tkm");

	m_position.y += 50.0f;
	m_moveDirection = m_player->GetPlayerDir();

	m_moveDirection.y = 0.0f;
	m_position += m_moveDirection * 30.0f;

	Vector3 size = { 300.0f,50.0f,100.0f };
	Vector3 modelSize = Vector3::One;

	if (m_isCharge) {
		if (m_powerUp == enPowerUp2) {
			float s = 2.0f;
			size *= s;
			modelSize *= s;
		}
		if (m_powerUp == enPowerUp1) {
			for (int i = 0;i < 2;i++)
			{
				m_subModel[i].Init("Assets/modelData/player/slash.tkm");

				m_vec[i] = m_player->GetPlayerPos();
				m_rot[i] = m_moveDirection;

				Quaternion rot;
				rot.SetRotationYFromDirectionXZ(m_rot[i]);
				if (i == 0) {
					rot.AddRotationDegY(25.0f);
				}
				else {
					rot.AddRotationDegY(-25.0f);
				}
				rot.Apply(m_rot[i]);

				m_vec[i] += m_rot[i] * 30.0f;

				m_subModel[i].SetPosition(m_vec[i]);
				m_subModel[i].SetScale({ 0.7f,0.7f,0.7f });
				m_subModel[i].SetRotation(rot);

				m_subCollision[i] = NewGO<CollisionObject>(0);
				m_subCollision[i]->CreateBox(m_vec[i], rot, (size * 0.7f));
				m_subCollision[i]->SetTimeLimit(DELETE_TIME);
				m_subCollision[i]->SetPosition(m_vec[i]);
				m_projectileManager->AddProjectile(m_subCollision[i]);
			}
		}
	}

	Quaternion rot;
	rot.SetRotationYFromDirectionXZ(m_moveDirection);
	m_mainCollision = NewGO<CollisionObject>(0);
	m_mainCollision->CreateBox(m_position, rot, size);
	m_mainCollision->SetTimeLimit(DELETE_TIME);
	m_mainCollision->SetPosition(m_position);
	m_projectileManager->AddProjectile(m_mainCollision);

	m_sphereModel.SetPosition(m_position);
	m_sphereModel.SetRotation(rot);
	m_sphereModel.SetScale(modelSize);

	return true;
}

void SlashAttack::Update()
{
	if (m_mainCollision->IsDead()) {
		m_projectileManager->DeleteProjectile(m_mainCollision);
		if (m_powerUp = enPowerUp1){
			m_projectileManager->DeleteProjectile(m_subCollision[0]);
			m_projectileManager->DeleteProjectile(m_subCollision[1]);
		}
		DeleteGO(this);
		return;
	}
	if (m_powerUp == enPowerUp1){
		for (int i = 0;i < 2;i++) {
			m_vec[i] += m_rot[i] * MOVE_SPEED_OFFSET;
			m_subCollision[i]->SetPosition(m_vec[i]);
			m_subModel[i].SetPosition(m_vec[i]);

			m_subModel[i].Update();
		}
	}

	m_position += m_moveDirection * MOVE_SPEED_OFFSET;

	m_mainCollision->SetPosition(m_position);
	m_sphereModel.SetPosition(m_position);

	m_sphereModel.Update();

}



void SlashAttack::Render(RenderContext& rc)
{
	m_sphereModel.Draw(rc);
	if (m_powerUp == enPowerUp1) {
		m_subModel[0].Draw(rc);
		m_subModel[1].Draw(rc);
	}
}


