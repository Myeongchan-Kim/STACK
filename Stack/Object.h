#pragma once
#include <DirectXMath.h>
#include <functional>
#include <list>

using namespace DirectX;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Play(float dt);
	virtual void AddMoveToScheduler(float x, float y, float z, float time);
	void StopMove();
	bool IsOnMove() { return m_moveList.size() > 0; }
	virtual void MoveBy(float x, float y, float z);
	virtual const XMFLOAT3 GetPosition();
	
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	float GetScaleX();
	float GetScaleY();
	float GetScaleZ();

	struct LinearMove
	{
		float x, y, z;
		float remainTime;
	};

protected:
	XMFLOAT4 				m_pos = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4		m_scale;
	DirectX::XMFLOAT4		m_rgba;
	float					m_xRot;
	float					m_yRot;
	float					m_zRot;
	std::list<LinearMove>	m_moveList;
};

