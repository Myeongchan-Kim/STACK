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
	virtual bool Update(float dt, InputClass& input, Camera& camera) override;
	float GetHeight();
	int GetCount();

	void LoadUI();
	void UpdateUIString(Camera& camera) override;
	void UpdateUIPos(Camera& camera);
	void MoveCameraAndBackground(Camera& camera, float dy);
	ModelClass* MakeNewBlock(XMFLOAT3 positoin , XMFLOAT3 boxSize);


	static const float DEFAULT_VIEW_WIDTH;
	static const float DEFAULT_VIEW_HEIGHT;
	static const XMFLOAT3 DEFAULT_BOXSIZE;

private:
	void ChangeDirection();
	bool UpdatePlayState(float dt, InputClass& input, Camera& camera);
	bool UpdateEndingState(float dt, InputClass& input, Camera& camera);
	bool UpdateEndDoneState(float dt, InputClass& input, Camera& camera);
	bool UpdateRestarting(float dt, InputClass& input, Camera& camera);

	bool IsOn(ModelClass* b1, ModelClass* b2);
	bool IsExactFit(ModelClass* ubox, ModelClass* dbox);
	ModelClass* m_currentBlock = nullptr;
	ModelClass* m_lastBlock = nullptr;
	XMFLOAT3 m_curPos = { 0.0f, 0.0f, 0.0f };
	
	bool m_isEnd = false; //엔딩모드냐 아니냐
	bool m_isReadyToNewStart = false;
	enum playState
	{
		isPlaying,
		isEnding,
		isEndingDone,
		isRestarting
	} m_playState;

	XMFLOAT3 m_curMoveDir = { 0.0f, 0.0f, 0.0f };

	ModelClass* m_backGround = nullptr;
	XMFLOAT3 m_color;
	XMFLOAT3 m_boxSize = DEFAULT_BOXSIZE;


	PreLoadedData* m_uiPool[10];
	float m_currentHeight = 0.0f;
	int m_countAccumulation = 0;
	int m_randomSeed;
	int m_exactFitCount = 0;
	std::string m_scaleSounds[8];


};
