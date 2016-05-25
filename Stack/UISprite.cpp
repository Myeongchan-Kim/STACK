#include "UISprite.h"
#include "MyVertex.h"

const float UISprite::LETTERWIDTH = 0.1f;
const float UISprite::LETTERHEIGHT = 0.2f;

UISprite::UISprite() : m_width(LETTERHEIGHT), m_height(LETTERHEIGHT), m_uiPosx(0.0f), m_uiPosy(0.0f)
{
}


UISprite::~UISprite()
{
}

void UISprite::SetToNumber(int n)
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { -m_width / 2 , +m_height / 2, 0.0f, };
	pos[1] = { +m_width / 2 , +m_height / 2, 0.0f, };
	pos[2] = { +m_width / 2 , -m_height / 2, 0.0f, };
	pos[3] = { -m_width / 2 , -m_height / 2, 0.0f, };

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
		rgba = { 1.0f, 165.0f / 255.0f, 0.0f, 1.0f };
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

	float startTexPos = (float)n / 14.0f;
	float finishTexPos = (float)(n + 1) / 14.0f;
	MyVertex v1 = { pos[0], rgba, normal,{ startTexPos, 0.0f } };
	MyVertex v2 = { pos[1], rgba, normal,{ finishTexPos, 0.0f } };
	MyVertex v3 = { pos[2], rgba, normal,{ finishTexPos, 1.0f } };
	MyVertex v4 = { pos[3], rgba, normal,{ startTexPos, 1.0f } };

	AddRectangle(v1, v2, v3, v4);
}

void UISprite::SetUIXY(float x, float y)
{
	m_uiPosx = x;
	m_uiPosy = y;
}
