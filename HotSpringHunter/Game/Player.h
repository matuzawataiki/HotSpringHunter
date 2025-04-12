#pragma once
class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start()override;
	void Update()override;
	//player移動
	void Move();
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
	//player速度ベクトルのゲッター。
	Vector3 GetPlayerSpeed() {
		return m_playerSpeed;
	}

private:
	ModelRender m_playerModelRender;						//player描画。
	Vector3 m_playerPosition = Vector3::Zero;				//player座標。
	Vector3 m_playerSpeed = Vector3::Zero;					//player移動スピード。
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
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];

	int m_animationState = 0;			//playerアニメーションの状態。
	enum EnPlayerAnimVar {
		enIdle,
		enWalk,
		enRun,
		enJump,
		enGuardStart,
		enGuardEnd,
		enWeakAttack
	};
};