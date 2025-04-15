#pragma once
class Player :public IGameObject
{

public:
	Player();
	~Player();
	bool Start()override;
	void LoadModel();
	void GenerateMinions();
	void Update()override;
	//player移動
	void Move();
	//playerの向きを計算。
	void GetDirection(Vector3 foward,Vector3 right);
	//player移動速度調整。
	void MoveAdjust();
	//player回転。
	void Rotation();
	//state管理。
	void StateManage();
	//playerアニメーション。
	void AnimationManage();
	void Render(RenderContext& rc)override;

	//player座標のゲッター。
	Vector3 GetPlayerPosition() {
		return m_playerPosition;
	}
	//playerの向きのゲッター。
	Vector3 GetPlayerDirection() {
		return m_playerDirection;
	}

	int m_animationState = 0;			//playerアニメーションの状態。
	enum EnPlayerAnimVar {
		enIdle,
		enWalk,
		enRun,
		enJump,
		enGuardStart,
		enGuardEnd,
		enWeakAttack,
		enHit,
		enDeath,
	};

private:
	ModelRender m_playerModelRender;						//player描画。
	Vector3 m_playerPosition = Vector3::Zero;				//player座標。
	Vector3 m_playerSpeed = Vector3::Zero;					//player移動スピード。
	Vector3 m_playerDirection = Vector3::Zero;				//player向き。
	Quaternion m_playerRotation = Quaternion::Identity;		//player回転。
	CharacterController m_playerCharaCon;					//playerキャラコン。

	float m_runState = 1.0f;								//player走り状態の管理。
	float m_guardState = 1.0f;								//ガード状態の管理。

	//アニメーション。
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_GuardStart,
		enAnimationClip_GuardEnd,
		enAnimationClip_WeakAttack,
		enAnimationClip_Hit,
		enAnimationClip_Death,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];
};