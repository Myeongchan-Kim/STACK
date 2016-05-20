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

void Object::AddLinearMoveToScheduler(float x, float y, float z, float time)
{
	LinearMove* move = new LinearMove(x, y, z, time);
	m_moveList.push_back(move);
}

void Object::AddGravityMoveToScheduler(XMFLOAT3 initialV, float time)
{
	GravityMove* move = new GravityMove(initialV, time);
	m_moveList.push_back(move);
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
		XMFLOAT3& move = (*m_moveList.begin())->GetDeltaPosition(dt);
		MoveBy(move.x, move.y, move.z);
		(*m_moveList.begin())->remainTime -= dt;


		auto negative = [](Move* m) 
		{ 
			bool result = m->remainTime <= 0.0f;
			if (result)
			{
				delete m;
				m = nullptr;
			}
			return  result;
		};
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

XMFLOAT3 Object::LinearMove::GetDeltaPosition(float dt)
{
	XMFLOAT3 velocity = { x / remainTime, y / remainTime, z / remainTime };
	XMFLOAT3 dPos;
	if (remainTime >= dt)
	{
		dPos = { velocity.x * dt, velocity.y * dt, velocity.z * dt };
		x -= velocity.x * dt;
		y -= velocity.y * dt;
		z -= velocity.z * dt;
	}
	else
	{
		dPos = { x,y,z };
	}
	return dPos;
}

XMFLOAT3 Object::GravityMove::GetDeltaPosition(float dt)
{
	v.y -= G*dt;	
	XMFLOAT3 resultDeltaPos = { v.x * dt, v.y * dt, v.z * dt};
	return resultDeltaPos;
}
