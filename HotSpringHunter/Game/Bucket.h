#pragma once
class Player;
class Bucket:public IGameObject
{
public:
	Bucket();
	~Bucket();
	bool Start()override;
	void LoadAssets();					//assets読み込み。
	void Update()override;
	void FollowPlayer();				//player追従。
	void ManageAnimState();				//アニメーションステート管理。
	void DisplayManage();				//表示状態切り替え。
	void Render(RenderContext& rc)override;

private:
	Player* m_player = nullptr;

	ModelRender m_bucketModel;
	Quaternion m_bucketRot = Quaternion::Identity;
	Vector3 m_bucketPos = Vector3::Zero;				//桶位置。
	Vector3 m_bucketDir = Vector3::Zero;				//桶向き。

	bool m_displayFlag = false;							//桶表示のフラッグ。
	int m_animState = 0;								//アニメーションステート。

	enum EnAnimClip{							//アニメーションデータ。
		enAnimClip_GuardStart,
		enAnimClip_GuardEnd,
		enAnimClip_Num,
	};
	AnimationClip m_animClips[enAnimClip_Num];

	enum EnAnimState {							//アニメーションステート。
		enGuardStart,
		enGuardEnd,
	};
};

