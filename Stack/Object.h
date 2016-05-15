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

	virtual void Frame(float dt);
	virtual void AddMoveToScheduler(float x, float y, float z, float time);
	virtual void MoveTo(float x, float y, float z);

	struct LinearMove
	{
		float x, y, z;
		float remainTime;
	};

protected:

	XMFLOAT4 				m_pos;
	std::list<LinearMove>	m_moveList;
};

