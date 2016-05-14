#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void InitCamera();
	void MoveCamera(float x, float y, float z);
	XMMATRIX GetView();
	XMMATRIX GetProjection(float width, float height);
private:
	XMFLOAT4 			m_pos;
	XMFLOAT4 			m_target;
	XMFLOAT4 			m_up;

	XMMATRIX			m_view;
	XMMATRIX			m_projection;
};

