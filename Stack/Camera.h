#pragma once
#include <DirectXMath.h>
#include "Object.h"
using namespace DirectX;

class Camera : public Object
{
public:
	Camera();
	virtual ~Camera();

	void InitCamera();
	XMMATRIX GetView();
	XMMATRIX GetProjection();
	float GetViewSizeWidth();
	float GetViewSizeHeight();

	XMFLOAT3 GetVewDir();
	XMFLOAT3 GetNormalNiewDIr();
	void SetProjection(float width, float height);
	virtual void MoveBy(float x, float y, float z);

	void SetCameraPos(float x, float y, float z);
	void SetCameraTarget(float x, float y, float z);
	void MoveCameraFor(float x, float y, float z, float time);

private:
	XMFLOAT4 			m_target;
	XMFLOAT4 			m_up;
	float				m_width;
	float				m_height;

	XMMATRIX			m_view;
	XMMATRIX			m_projection;

};

