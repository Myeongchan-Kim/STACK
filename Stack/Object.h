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
	virtual void MoveBy(float x, float y, float z);
	virtual XMFLOAT3 GetPosition();
	
	struct LinearMove
	{
		float x, y, z;
		float remainTime;
	};

protected:

	XMFLOAT4 				m_pos = {0.0f, 0.0f, 0.0f, 0.0f};
	std::list<LinearMove>	m_moveList;
};

