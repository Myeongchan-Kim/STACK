#include "Object.h"
#include "Camera.h"


Object::Object() : m_xRot(0.0f), m_yRot(0.0f), m_zRot(0.0f), m_rgba(0.8f, 0.8f, 0.8f, 1.0f), m_scale(1.0f, 1.0f, 1.0f, 1.0f)
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

void Object::StopMove()
{
	m_moveList.clear();
}

void Object::Play(float dt)
{
	//이동 해야하는것 처리.
	if (!m_moveList.empty())
	{
		LinearMove& move = *m_moveList.begin();

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

		auto negative = [](const LinearMove& m) { return m.remainTime <= 0.0f; };
		m_moveList.remove_if(negative);
	}
		

}


const XMFLOAT3  const Object::GetPosition()
{
	return XMFLOAT3{ m_pos.x, m_pos.y, m_pos.z };
}

void Object::SetPosition(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Object::SetPosition(XMFLOAT3 pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}

void Object::SetRotation(float x, float y, float z)
{
	m_xRot = x;
	m_yRot = y;
	m_zRot = z;
}

void Object::SetScale(float x, float y, float z)
{
	m_scale = { x,y,z, 1.0f };
}

float Object::GetScaleX()
{
	return m_scale.x;
}

float Object::GetScaleY()
{
	return m_scale.y;
}

float Object::GetScaleZ()
{
	return m_scale.z;
}
