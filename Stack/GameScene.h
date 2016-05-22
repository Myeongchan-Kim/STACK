#pragma once
#include "Scene.h"
#include "ModelClass.h"
class GameScene :
	public Scene
{
public:
	~GameScene();
	virtual void Start(Camera& camera) override;
	void SetSounds();
	virtual void Update(float dt, InputClass& input, Camera& camera) override;
	float GetHeight();
	int GetCount();

	void LoadUI();
	void UpdateUIString(Camera& camera) override;
	void UpdateUIPos(Camera& camera);
	void MoveCameraAndBackground(Camera& camera, float dy);
	ModelClass* MakeNewBlock(XMFLOAT3 positoin , XMFLOAT3 boxSize);

	static const float DEFAULT_VIEW_WIDTH;
	static const float DEFAULT_VIEW_HEIGHT;

private:
	void ChangeDirection();

	bool IsOn(ModelClass* b1, ModelClass* b2);
	bool IsExactFit(ModelClass* ubox, ModelClass* dbox);
	ModelClass* m_currentBlock = nullptr;
	ModelClass* m_lastBlock = nullptr;
	XMFLOAT3 m_curPos = { 0.0f, 0.0f, 0.0f };
	bool m_isEnd = false; //엔딩모드냐 아니냐
	XMFLOAT3 m_curMoveDir = { 0.0f, 0.0f, 0.0f };

	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
	static const XMFLOAT3 DEFAULT_BOXSIZE;
	XMFLOAT3 m_boxSize = DEFAULT_BOXSIZE;


	PreLoadedData* m_uiPool[10];
	float m_currentHeight = 0.0f;
	int m_countAccumulation = 0;
	int m_randomSeed;
	int m_exactFitCount = 0;
	std::string m_scaleSounds[8];


};
