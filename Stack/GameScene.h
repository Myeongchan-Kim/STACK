#pragma once
#include "Scene.h"
#include "ModelClass.h"
class GameScene :
	public Scene
{
public:

	virtual void Start(Camera& camera) override;
	virtual void Update(InputClass& input, Camera& camera) override;

private:
	ModelClass* m_currentBlock = nullptr;
	ModelClass* m_lastBlock = nullptr;
	Vector3 m_curPos = { 0, 0, -4 };


	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
	Vector3 m_boxSize = { 2, 1, 2 };
};

