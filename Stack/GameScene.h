#pragma once
#include "Scene.h"
#include "ModelClass.h"
class GameScene :
	public Scene
{
public:

	virtual void Start(Camera& camera) override;
	virtual void Update(float dt, InputClass& input, Camera& camera) override;
	float GetHeight();
	int GetCount();
	void UpdateUI(Camera& camera) override;

	static const float DEFAULT_VIEW_WIDTH;
	static const float DEFAULT_VIEW_HEIGHT;

private:
	void ChangeDirection();

	bool IsOn(ModelClass* b1, ModelClass* b2);
	ModelClass* m_currentBlock = nullptr;
	ModelClass* m_lastBlock = nullptr;
	XMFLOAT3 m_curPos = { 0.0f, 0.0f, 0.0f };
	bool m_isEnd = false; //엔딩모드냐 아니냐
	XMFLOAT3 m_curMoveDir = { 0.0f, 0.0f, 0.0f };

	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
	XMFLOAT3 m_boxSize = { 4, 1.0, 4 };

	float m_currentHeight = 0.0f;
	int m_countAccumulation = 0;
};
