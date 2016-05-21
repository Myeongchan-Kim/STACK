#include <fstream>
#include "UIModel.h"
#include "MyVertex.h"
#include "ConstVars.h"


const float UIModel::LETTERWIDTH = 0.5f;
const float UIModel::LETTERHEIGHT = 1.0f;

UIModel::UIModel() : ModelClass(), m_uiPosx(0), m_uiPosy(0), m_width(LETTERWIDTH), m_height(LETTERHEIGHT)
{
}


UIModel::~UIModel()
{
}


void UIModel::SetToNumber(int n)
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { -m_width / 2 ,0.0f, +m_height / 2, };
	pos[1] = { +m_width / 2 ,0.0f, +m_height / 2, };
	pos[2] = { +m_width / 2 ,0.0f, -m_height / 2, };
	pos[3] = { -m_width / 2 ,0.0f, -m_height / 2, };
	
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };
	
	XMFLOAT4 rgba;
	switch (n)
	{
	case 0:
		//red
		rgba = { 1.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 1:
		//orange
		rgba = { 1.0f, 165.0f/255.0f, 0.0f, 1.0f };
		break;
	case 2:
		//yellow
		rgba = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 3:
		//green
		rgba = { 0.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 4:
		//skyblue
		rgba = { 0.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 5:
		//blue
		rgba = { 0.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 6:
		//dark blue
		rgba = { 0.0f, 0.0f, 0.6f, 1.0f };
		break;
	case 7:
		//purple
		rgba = { 0.3f, 0.1f, 0.6f, 1.0f };
		break;
	case 8:
		//black
		rgba = { 0.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 9:
		//white
		rgba = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	default:
		break;
	}
	
	MyVertex v1 = { pos[0], rgba, normal,{ 0.0f, 0.0f } };
	MyVertex v2 = { pos[1], rgba, normal,{ 1.0f, 0.0f } };
	MyVertex v3 = { pos[2], rgba, normal,{ 1.0f, 1.0f } };
	MyVertex v4 = { pos[3], rgba, normal,{ 0.0f, 1.0f } };

	AddRectangle(v1, v2, v3, v4);
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

	float deltaX = (m_uiPosx * camera.GetViewSizeWidth() - camera.GetViewSizeWidth() / 2.0f);
	float deltaY = (m_uiPosy * camera.GetViewSizeHeight() - camera.GetViewSizeHeight() / 2.0f);

	XMVECTOR v = { centerPos.x, centerPos.y, centerPos.z};

	//xz평면으로 돌리는 행렬
	XMMATRIX mat = XMMatrixRotationZ(-m_zRot);
	mat *= XMMatrixRotationY(-m_yRot);
	mat *= XMMatrixRotationX(-m_xRot);

	//XZ평면에서 움직임
	mat *= XMMatrixTranslation(deltaX, deltaY, 0.0f);

	//다시 원래 각도로 돌리는 행렬
	mat *= XMMatrixRotationX(m_xRot);
	mat *= XMMatrixRotationY(m_yRot);
	mat *= XMMatrixRotationZ(m_zRot);

	//위치이동
	XMFLOAT3 resultPos;
	auto resultVec = XMVector3Transform(v, mat);
	XMStoreFloat3(&resultPos, resultVec);

	SetPosition(resultPos);
}

