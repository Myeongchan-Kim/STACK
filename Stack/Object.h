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
	virtual const XMFLOAT3 const GetPosition();
	
	struct LinearMove
	{
		float x, y, z;
		float remainTime;
	};

protected:

	XMFLOAT4 				m_pos = {0.0f, 0.0f, 0.0f, 0.0f};
	std::list<LinearMove>	m_moveList;
};

