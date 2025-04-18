#pragma once
class Player;
class PlayerChargeAttack;
class Towel:public IGameObject
{
public:
	Towel();
	~Towel();
	bool Start()override;
	void LoadModel();			//モデル読み込み。
	void Update()override;
	void FollowPlayer();		//player追従。
	void StateManage();			//ステート切り替え。
	void ChargeAttackScale();	//溜め攻撃時のタオルの大きさを変える。
	void DisplayManage();		//表示状態切り替え。
	void Render(RenderContext& rc)override;
private:
	Player* m_player = nullptr;
	PlayerChargeAttack* m_playerCharAt = nullptr;

	ModelRender m_towelModel;
	Quaternion m_towelRot = Quaternion::Identity;
	Vector3 m_towelDir = Vector3::Zero;				//タオル向き。
	Vector3 m_towelPos = Vector3::Zero;				//タオル位置。
	Vector3 m_towelSca = Vector3::Zero;				//タオル大きさ。

	bool m_displayFlag = true;			//タオル表示のフラッグ。
	int m_animState = 0;			//アニメーションステート。

	enum EnAnimState {
		WeakAttack,
		ChargeAttack,
		Charging,
	};
	enum EnAnimClip{
		enAnimClip_WeakAttack,
		enAnimClip_ChargeAttack,
		enAnimClip_Charging,
		enAnimClip_Num,
	};
	AnimationClip m_animClips[enAnimClip_Num];	
};

