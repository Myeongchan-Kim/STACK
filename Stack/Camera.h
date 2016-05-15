#pragma once
#include <DirectXMath.h>
#include "Object.h"
using namespace DirectX;

class Camera : public Object
{
public:
	Camera();
	~Camera();

	void InitCamera();
	XMMATRIX GetView();
	XMMATRIX GetProjection(float width, float height);
	virtual void MoveBy(float x, float y, float z);

private:
	XMFLOAT4 			m_target;
	XMFLOAT4 			m_up;

	XMMATRIX			m_view;
	XMMATRIX			m_projection;

};

