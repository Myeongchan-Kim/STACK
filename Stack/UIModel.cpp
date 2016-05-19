#include "UIModel.h"



UIModel::UIModel():m_uiPosx(0), m_uiPosy(0)
{
}


UIModel::~UIModel()
{
}

void UIModel::SetUIXY(float x, float y)
{
	m_uiPosx = x;
	m_uiPosy = y;
}

float UIModel::GetUIX()
{
	return m_uiPosx;
}

float UIModel::GetUIY()
{
	return m_uiPosy;
}

void UIModel::SetUIPosition(Camera & camera)
{
	auto cameraPos = camera.GetPosition();
	auto normalviewVec = camera.GetNormalNiewDIr();
	float nearPlaneDist = 1.0f;

	XMFLOAT3 centerPos = {
		cameraPos.x + normalviewVec.x / nearPlaneDist,
		cameraPos.y + normalviewVec.y / nearPlaneDist,
		cameraPos.z + normalviewVec.z / nearPlaneDist, };

	float deltaX = m_uiPosx - camera.GetViewSizeWidth()/2;
	float deltaY = m_uiPosy - camera.GetViewSizeHeight()/2;

	XMVECTOR v = { centerPos.x, centerPos.y, centerPos.z};

	//xz������� ������ ���
	XMMATRIX mat = XMMatrixRotationZ(-m_zRot);
	mat *= XMMatrixRotationY(-m_yRot);
	mat *= XMMatrixRotationX(-m_xRot);

	//XZ��鿡�� ������
	mat *= XMMatrixTranslation(deltaX, 0.0f, deltaY);

	//�ٽ� ���� ������ ������ ���
	mat *= XMMatrixRotationX(m_xRot);
	mat *= XMMatrixRotationY(m_yRot);
	mat *= XMMatrixRotationZ(m_zRot);

	//��ġ�̵�
	XMFLOAT3 resultPos;
	auto resultVec = XMVector3Transform(v, mat);
	XMStoreFloat3(&resultPos, resultVec);

	SetPosition(resultPos);
}

