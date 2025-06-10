#pragma once
class Fence;
class FenceManager:public IGameObject
{
public:
	FenceManager();
	~FenceManager();
	bool Start()override;
	void Update()override;

private:
};