#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}


void Object::MoveBy(float x, float y, float z)
{
	m_pos.x += x;
	m_pos.y += y;
	m_pos.z += z;
}

void Object::AddMoveToScheduler(float x, float y, float z, float time)
{
	m_moveList.push_back({ x, y, z, time });
}

void Object::Frame(float dt)
{
	//이동 해야하는것 처리.
	for (auto iter = m_moveList.begin(); iter != m_moveList.end(); iter++)
	{
		auto& move = *iter;
		XMFLOAT3 velocity = { move.x / move.remainTime, move.y / move.remainTime, move.z / move.remainTime };

		if (move.remainTime >= dt)
		{
			MoveBy(velocity.x * dt, velocity.y * dt, velocity.z * dt);
			move.x -= velocity.x * dt;
			move.y -= velocity.y * dt;
			move.z -= velocity.z * dt;
		}
		else
		{
			MoveBy(move.x, move.y, move.z);
		}

		move.remainTime -= dt;
	}
	auto negative = [](const LinearMove& m) { return m.remainTime <= 0.0f; };
	m_moveList.remove_if(negative);

	//기타 추가할 처리..
	//
}

