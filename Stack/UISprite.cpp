#include "UISprite.h"
#include "MyVertex.h"

const float UISprite::LETTERWIDTH = 0.10f;
const float UISprite::LETTERHEIGHT = 0.15f;

UISprite::UISprite() : m_width(UISprite::LETTERWIDTH), m_height(UISprite::LETTERHEIGHT), m_uiPosx(0.0f), m_uiPosy(0.0f)
{
}


UISprite::~UISprite()
{
}

void UISprite::SetToRect()
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { m_uiPosx - m_width / 2 , m_uiPosy + m_height / 2, 0.0f, };
	pos[1] = { m_uiPosx + m_width / 2 , m_uiPosy + m_height / 2, 0.0f, };
	pos[2] = { m_uiPosx + m_width / 2 , m_uiPosy - m_height / 2, 0.0f, };
	pos[3] = { m_uiPosx - m_width / 2 , m_uiPosy - m_height / 2, 0.0f, };

	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };

	XMFLOAT4 rgba = { 1.0f, 1.0f, 1.0f, 1.0f };

	MyVertex v1 = { pos[0], rgba, normal,{ 0.0f, 0.0f } };
	MyVertex v2 = { pos[1], rgba, normal,{ 1.0f, 0.0f } };
	MyVertex v3 = { pos[2], rgba, normal,{ 1.0f, 1.0f } };
	MyVertex v4 = { pos[3], rgba, normal,{ 0.0f, 1.0f } };

	AddRectangle(v1, v2, v3, v4);
}


void UISprite::SetToNumber(int n)
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { m_uiPosx - m_width / 2 , m_uiPosy + m_height / 2, 0.0f, };
	pos[1] = { m_uiPosx + m_width / 2 , m_uiPosy + m_height / 2, 0.0f, };
	pos[2] = { m_uiPosx + m_width / 2 , m_uiPosy - m_height / 2, 0.0f, };
	pos[3] = { m_uiPosx - m_width / 2 , m_uiPosy - m_height / 2, 0.0f, };

	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };

	XMFLOAT4 rgba = { 1.0f, 1.0f, 1.0f, 1.0f };

	float startTexPos = (float)n / 37.0f;
	float finishTexPos = (float)(n + 1) / 37.0f;
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
