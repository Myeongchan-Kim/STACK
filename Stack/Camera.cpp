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
	m_width = 100;
	m_height = 100;
	m_projection = XMMatrixOrthographicLH(m_width, m_height, 0.1f, 1000.0f); // near plane, far plane
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
	AddLinearMoveToScheduler(x, y, z, time);
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

float Camera::GetViewSizeWidth()
{
	return m_width;
}

float Camera::GetViewSizeHeight()
{
	return m_height;
}


XMFLOAT3 Camera::GetVewDir()
{
	XMFLOAT3 viewDir(m_target.x -  m_pos.x, m_target.y - m_pos.y, m_target.z - m_pos.z);
	return viewDir;
}

XMFLOAT3 Camera::GetNormalNiewDIr()
{
	auto vecLen = [](XMFLOAT3 v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	};
	auto normalize = [=](XMFLOAT3 v)
	{
		return XMFLOAT3({ v.x / vecLen(v), v.y / vecLen(v), v.z / vecLen(v) });
	};
	return normalize(GetVewDir());;
}

void Camera::SetProjection(float width, float height)
{
	m_width = width;
	m_height = height;
	m_projection = XMMatrixOrthographicLH(width, height, 0.1f, 1000.0f); // near plane, far plane
}

