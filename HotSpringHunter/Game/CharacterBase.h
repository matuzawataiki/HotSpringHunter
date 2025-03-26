#pragma once

//キャラクター基底クラス。

class Player;
class CharacterBase:public IGameObject
{
public:
	CharacterBase();
	~CharacterBase();

	bool Start()override;
	void Update()override;
	//キャラの基本移動（キャラの移動、重力）。
	void BaseMove(Vector3 position, Vector3 speed);
	//キャラの回転。
	Quaternion BaseRotation(Quaternion rotation, Vector3 speed);
};

