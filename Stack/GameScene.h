#pragma once
#include "Scene.h"
#include "ModelClass.h"
class GameScene :
	public Scene
{
public:

	virtual void Start(Camera& camera) override;
	virtual void Update(float dt, InputClass& input, Camera& camera) override;

private:
	bool IsOn(ModelClass* b1, ModelClass* b2);
	ModelClass* m_currentBlock = nullptr;
	ModelClass* m_lastBlock = nullptr;
	Vector3 m_curPos = { 0, 0, 0 };
	bool m_isEnd = false;

	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
	Vector3 m_boxSize = { 3, 1, 3 };
};
