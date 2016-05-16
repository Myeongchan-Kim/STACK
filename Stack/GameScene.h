#pragma once
#include "Scene.h"
class GameScene :
	public Scene
{
public:

	virtual void Start() override;
	virtual void Update(InputClass& input, Camera& camera) override;

private:
	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
};

