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
	virtual void AddLinearMoveToScheduler(float x, float y, float z, float time);
	virtual void AddGravityMoveToScheduler(XMFLOAT3 initialV, float time);
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
	XMFLOAT3 GetRotation();
	
	struct Move
	{
		Move(float remainTime) :remainTime(remainTime) {};
		virtual ~Move() {};
		virtual XMFLOAT3 GetDeltaPosition(float dt) = 0 ;
		float remainTime;
	};

	struct LinearMove : public Move
	{
		LinearMove(float x, float y, float z, float remainTime):Move(remainTime), x(x),y(y),z(z) {};
		float x, y, z;
		XMFLOAT3 GetDeltaPosition(float dt) override;
	};

	struct GravityMove : public Move
	{
		GravityMove(XMFLOAT3 initialV, float remainTime, Object* parent) : Move(remainTime), initialV(initialV), v(initialV), parent(parent) {};
		Object* parent;
		const XMFLOAT3 initialV;
		XMFLOAT3 v;
		const float G = 40.0f;
		float elapsedTime = 0.0f;
		XMFLOAT3 GetDeltaPosition(float dt) override;
	};

protected:
	XMFLOAT4 				m_pos = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4		m_scale;
	DirectX::XMFLOAT4		m_rgba;
	float					m_xRot;
	float					m_yRot;
	float					m_zRot;
	std::list<Move*>	m_moveList;
};

