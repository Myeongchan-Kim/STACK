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

	void SetCameraPos(float x, float y, float z);
	void SetCameraTarget(float x, float y, float z);
	void MoveCameraFor(float x, float y, float z, float time);
private:
	XMFLOAT4 			m_target;
	XMFLOAT4 			m_up;

	XMMATRIX			m_view;
	XMMATRIX			m_projection;

};

