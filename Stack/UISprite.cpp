#include "UISprite.h"
#include "MyVertex.h"

const float UISprite::LETTERWIDTH = 0.10f;
const float UISprite::LETTERHEIGHT = 0.15f;
std::map<char, UISprite::LetterWidth> UISprite::DEFAULT_LETTER_WIDTH = 
{
	{ '0', { 0.0f / 2000.0f, 100.0f / 2000.0f } },
	{ '1', { 100.0f / 2000.0f , 160.0f / 2000.0f} },
	{ '2', { 160.0f / 2000.0f , 260.0f / 2000.0f} },
	{ '3', { 260.0f / 2000.0f , 356.0f / 2000.0f} },
	{ '4', { 356.0f / 2000.0f , 454.0f / 2000.0f} },
	{ '5', { 454.0f / 2000.0f , 546.0f / 2000.0f} },
	{ '6', { 546.0f / 2000.0f , 634.0f / 2000.0f} },
	{ '7', { 634.0f / 2000.0f , 720.0f / 2000.0f} },
	{ '8', { 720.0f / 2000.0f , 810.0f / 2000.0f} },
	{ '9', { 810.0f / 2000.0f , 906.0f / 2000.0f} },
};

UISprite::UISprite() : m_width(LETTERWIDTH), m_height(LETTERHEIGHT), m_uiPosx(0.0f), m_uiPosy(0.0f)
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

void UISprite::SetToChar(char c)
{
	auto letterPos = DEFAULT_LETTER_WIDTH.find(c);
	float startTexPos;
	float finishTexPos;

	if (letterPos == DEFAULT_LETTER_WIDTH.end())
	{
		throw "no character inside infomap";
		return;
	}
	else
	{
		startTexPos = (letterPos->second).startPos;
		finishTexPos = (letterPos->second).finishPos;
		m_height = 0.2f;
		m_width = (finishTexPos - startTexPos) * m_height * 13.0f;
	}

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
