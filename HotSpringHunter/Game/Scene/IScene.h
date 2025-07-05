#pragma once
#include "Util/CRC32.h"

#define appScene(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

//基底クラス
class IScene
{
public:
	IScene() {}
	virtual ~IScene() {} //仮想デストラクタ:派生クラスのデストラクタが呼ばれるようにする

	virtual bool Start()   = 0; //純粋仮想関数:派生クラスで実装されてなかったらエラーが出る
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	virtual bool RequestScene(uint32_t& id) = 0; //次のシーンを要求する。idは次のシーンのIDを返す。trueならば要求成功、falseならば失敗
};

