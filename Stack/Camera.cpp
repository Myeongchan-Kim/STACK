#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::InitCamera()
{
	m_projection = XMMatrixOrthographicLH(10, 10, 0.1f, 1000.0f); // near plane, far plane
	m_pos = { 0.0f, 0.0f, -10.0f, 1.0f };
	m_target = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_up = { 0.0f, 1.0f, 0.0f, 0.0f };
}

void Camera::MoveBy(float x, float y, float z)
{
	m_pos.x += x;
	m_pos.y += y;
	m_pos.z += z;

	m_target.x += x;
	m_target.y += y;
	m_target.z += z;
}


void Camera::SetCameraPos(float x, float y, float z)
{
	m_pos = { x, y, z, 1.0f };
}

void Camera::SetCameraTarget(float x, float y, float z)
{
	m_target = XMFLOAT4(x, y, z, 1.0f);
}

void Camera::MoveCameraFor(float x, float y, float z, float time)
{
	AddMoveToScheduler(x, y, z, time);
}

XMMATRIX Camera::GetView()
{
	auto pos = XMVectorSet(m_pos.x, m_pos.y, m_pos.z, m_pos.w);
	auto target = XMVectorSet(m_target.x, m_target.y, m_target.z, m_target.w);
	auto up = XMVectorSet(m_up.x, m_up.y, m_up.z, m_up.w);
	return XMMatrixLookAtLH(pos, target, up);
}

DirectX::XMMATRIX Camera::GetProjection()
{
	 	
	return m_projection;
}


void Camera::SetProjection(float width, float height)
{

	m_projection = XMMatrixOrthographicLH(width, height, 0.1f, 1000.0f); // near plane, far plane
}

