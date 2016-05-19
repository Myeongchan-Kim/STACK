#include <dxgi.h>
#include <d3dcompiler.h>
#include <algorithm>
#include "ModelClass.h"
#include "MyVertex.h"
#include "WICTextureLoader.h"
#include "ConstVars.h"

ModelClass::ModelClass() : m_indexBuffer(nullptr), m_vertexBuffer(nullptr), m_textureName(ConstVars::PLANE_TEX_FILE)
{

}

ModelClass::~ModelClass()
{
}


HRESULT ModelClass::CreateVertexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_vertices.size() * sizeof(MyVertex);			//버퍼 크기
	bd.Usage = D3D11_USAGE_DEFAULT;				//버퍼 사용 방식
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//파이프라인에 연결되는 버퍼 형태
	bd.CPUAccessFlags = 0;						//cpu접근 flag. 일반적으로 GPU를 사용하기 때문에 0을 쓴다.

	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory(&initData, sizeof(initData));
	if (m_vertices.empty())
	{
		assert("Model is not initailized. no vertices in model.");
	}
	initData.pSysMem = &m_vertices[0];				//초기화하기 위한 버퍼 배열 포인터

	if(m_vertexBuffer)
		ReleaseVB();
	return device->CreateBuffer(&bd,			//생성할 버퍼의 정보를 담은 구조체
		&initData,								//버퍼 초기화시 필요한 데이터
		&m_vertexBuffer);						//생성된 버퍼
}


HRESULT ModelClass::CreateIndexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = m_indices.size() * sizeof(WORD);				//버퍼 크기
	ibd.Usage = D3D11_USAGE_IMMUTABLE;				//버퍼 사용 방식
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//파이프라인에 연결되는 버퍼 형태
	ibd.CPUAccessFlags = 0;							//cpu접근 flag. 일반적으로 GPU를 사용하기 때문에 0을 쓴다.

	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_indices[0];						//초기화하기 위한 버퍼 배열 포인터
	if(m_indexBuffer)
		m_indexBuffer->Release();
	return device->CreateBuffer(&ibd,				//생성할 버퍼의 정보를 담은 구조체
		&initData,									//버퍼 초기화시 필요한 데이터
		&m_indexBuffer);							//생성된 버퍼
}

void ModelClass::SetTransParency(float alpha)
{
	for (auto& vertex : m_vertices)
	{
		vertex.color.w = alpha;
	}
}

void ModelClass::ReleaseVB()
{
	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
	}
}

void ModelClass::ReleaseIB()
{
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
	}
}

void ModelClass::SetSample()
{
	m_vertices.clear();

	MyVertex	vertices[] =
	{
		{ XMFLOAT3(-1.0f, 0.5f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(-0.33f, 0.33f, -0.33f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 0.5f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT3(0.33f, 0.33f, -0.33f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.33f, 0.33f, 0.33f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(-0.33f, 0.33f, 0.33f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -0.5f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(-0.33f, -0.33f, -0.33f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -0.5f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT3(0.33f, -0.33f, -0.33f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.33f, -0.33f, 0.33f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(-0.33f, -0.33f, 0.33f), XMFLOAT2(0.0f, 1.0f) },
	};

	int count = sizeof(vertices) / sizeof(MyVertex);
	for (int i = 0; i < count; i++)
	{
		m_vertices.push_back(vertices[i]);
	}

	m_indices.clear();
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,
		0, 5, 4,
		1, 5, 0,
		3, 4, 7,
		0, 4, 3,
		1, 6, 5,
		2, 6, 1,
		2, 7, 6,
		3, 7, 2,
		6, 4, 5,
		7, 4, 6,

	};

	count = sizeof(indices) / sizeof(WORD);
	for (int i = 0; i < count; i++)
	{
		m_indices.push_back(indices[i]);
	}
}

void ModelClass::SetToCube(XMFLOAT3 boxSize)
{
	float widthX = boxSize.x;
	float height = boxSize.y; 
	float widthZ = boxSize.z;
	m_vertices.clear();
	m_indices.clear();

	//position of vertecis
	XMFLOAT3 pos[8];
	for (int i = 0; i < 8; i++)
	{
		UINT right = i / 4;	//right or left
		UINT upper = (i / 2 )%2;	//upper or lower
		UINT back = i % 2; //front or back	
		pos[i] = XMFLOAT3(
			0 - widthX / 2 + right*widthX,
			0 - height / 2 + upper*height,
			0 - widthZ / 2 + back*widthZ
		);
	}
	
	//color
	
	//normal vector
	XMFLOAT3 left	= { -0.33f, +0.00f, +0.00f };
	XMFLOAT3 right	= { +0.33f, +0.00f, +0.00f };
	XMFLOAT3 up		= { +0.00f, +0.33f, +0.00f };
	XMFLOAT3 down	= { +0.00f, -0.33f, +0.00f };
	XMFLOAT3 front	= { +0.00f, +0.00f, -0.33f };
	XMFLOAT3 back	= { +0.00f, +0.00f, +0.33f };

	//top area
	auto v1 = MyVertex({ pos[0 + 2 + 1] , m_rgba, up,{ 0.0f, 0.0f } });
	auto v2 = MyVertex({ pos[4 + 2 + 1] , m_rgba, up,{ 1.0f, 0.0f } });
	auto v3 = MyVertex({ pos[4 + 2 + 0] , m_rgba, up,{ 1.0f, 1.0f } });
	auto v4 = MyVertex({ pos[0 + 2 + 0] , m_rgba, up,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);

	//bottom area
	v1 = MyVertex({ pos[0 + 0 + 0] , m_rgba, down,{ 0.0f, 0.0f } });
	v2 = MyVertex({ pos[4 + 0 + 0] , m_rgba, down,{ 1.0f, 0.0f } });
	v3 = MyVertex({ pos[4 + 0 + 1] , m_rgba, down,{ 1.0f, 1.0f } });
	v4 = MyVertex({ pos[0 + 0 + 1] , m_rgba, down,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);

	//right area
	v1 = MyVertex({ pos[4 + 2 + 0] , m_rgba, right,{ 0.0f, 0.0f } });
	v2 = MyVertex({ pos[4 + 2 + 1] , m_rgba, right,{ 1.0f, 0.0f } });
	v3 = MyVertex({ pos[4 + 0 + 1] , m_rgba, right,{ 1.0f, 1.0f } });
	v4 = MyVertex({ pos[4 + 0 + 0] , m_rgba, right,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);

	//left area
	v1 = MyVertex({ pos[0 + 2 + 1] , m_rgba, left,{ 0.0f, 0.0f } });
	v2 = MyVertex({ pos[0 + 2 + 0] , m_rgba, left,{ 1.0f, 0.0f } });
	v3 = MyVertex({ pos[0 + 0 + 0] , m_rgba, left,{ 1.0f, 1.0f } });
	v4 = MyVertex({ pos[0 + 0 + 1] , m_rgba, left,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);

	//front area
	v1 = MyVertex({ pos[0 + 2 + 0] , m_rgba, front,{ 0.0f, 0.0f } });
	v2 = MyVertex({ pos[4 + 2 + 0] , m_rgba, front,{ 1.0f, 0.0f } });
	v3 = MyVertex({ pos[4 + 0 + 0] , m_rgba, front,{ 1.0f, 1.0f } });
	v4 = MyVertex({ pos[0 + 0 + 0] , m_rgba, front,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);

	//back area
	v1 = MyVertex({ pos[4 + 2 + 1] , m_rgba, back,{ 0.0f, 0.0f } });
	v2 = MyVertex({ pos[0 + 2 + 1] , m_rgba, back,{ 1.0f, 0.0f } });
	v3 = MyVertex({ pos[0 + 0 + 1] , m_rgba, back,{ 1.0f, 1.0f } });
	v4 = MyVertex({ pos[4 + 0 + 1] , m_rgba, back,{ 0.0f, 1.0f } });
	AddRectangle(v1, v2, v3, v4);
}

void ModelClass::SetToRectangle(float width, float height, XMFLOAT3 normal)
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	XMFLOAT4 rgba;
	pos[0] = { -width / 2 ,0.0f, +height / 2, };
	pos[1] = { +width / 2 ,0.0f, +height / 2, };
	pos[2] = { +width / 2 ,0.0f, -height / 2, };
	pos[3] = { -width / 2 ,0.0f, - height / 2, };
	
	//auto dot = [](XMFLOAT3 v1, XMFLOAT3 v2)
	//{
	//	return (v1.x * v2.x + v1.y*v2.y + v1.z*v2.z);
	//};
	//auto rotation = [&](float& x, float &y, float rad)
	//{
	//	x = x * cosf(rad) + y * sinf(rad);
	//	y = -x * sinf(rad) + y * cosf(rad);
	//};
	//auto radAxisY = acosf(dot({ normal.x, 0.0f, normal.z }, standardNormal) / sqrt(normal.x * normal.x + normal.z * normal.z)); // Y축으로의 회전량
	//auto radAxisZ = acosf(dot({ normal.x, normal.y, 0.0f }, standardNormal) / sqrt(normal.x * normal.x + normal.y * normal.y)); // Z축으로의 회전량

	MyVertex v1 = { pos[0],{ m_rgba.x - 0.3f,m_rgba.y - 0.3f,m_rgba.z - 0.3f, 1.0f }, normal,{ 0.0f, 0.0f } };
	MyVertex v2 = { pos[1], m_rgba, normal,{ 1.0f, 0.0f } };
	MyVertex v3 = { pos[2],{ m_rgba.x - 0.3f,m_rgba.y - 0.3f,m_rgba.z - 0.3f, 1.0f }, normal,{ 1.0f, 1.0f } };
	MyVertex v4 = { pos[3],{ m_rgba.x - 0.8f,m_rgba.y - 0.8f,m_rgba.z - 0.8f, 1.0f }, normal,{ 0.0f, 1.0f } };

	AddRectangle(v1, v2, v3, v4);

}

void ModelClass::SetToNumber(int n)
{
	m_vertices.clear();
	m_indices.clear();
	float width = 0.5f;
	float height = 1.0f;

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { -width / 2 ,0.0f, +height / 2, };
	pos[1] = { +width / 2 ,0.0f, +height / 2, };
	pos[2] = { +width / 2 ,0.0f, -height / 2, };
	pos[3] = { -width / 2 ,0.0f, -height / 2, };
	
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

void ModelClass::AddRectangle(MyVertex& v1, MyVertex& v2, MyVertex& v3, MyVertex& v4)
{
	UINT square[4];
	auto GetIndex = [&](UINT& polygon, MyVertex& v) {
		auto iter = std::find(m_vertices.begin(), m_vertices.end(), v);
		if (iter != m_vertices.end())
			polygon = iter - m_vertices.begin();
		else
		{
			polygon = m_vertices.size();
			m_vertices.push_back(v);
		}
	};
	GetIndex(square[0], v1);
	GetIndex(square[1], v2);
	GetIndex(square[2], v3);
	GetIndex(square[3], v4);

	//up triangle
	m_indices.push_back(square[0]);
	m_indices.push_back(square[1]);
	m_indices.push_back(square[2]);
	//down triangle
	m_indices.push_back(square[2]);
	m_indices.push_back(square[3]);
	m_indices.push_back(square[0]);
}

DirectX::XMFLOAT3 ModelClass::GetRotation()
{
	return DirectX::XMFLOAT3({m_xRot, m_yRot, m_zRot});
}

void ModelClass::SetTextureName(WCHAR * textureName)
{
	m_textureName = textureName;
}

void ModelClass::SetColor(float r, float g, float b, float a)
{
	m_rgba.x = r;
	m_rgba.y = g;
	m_rgba.z = b;
	m_rgba.w = a;

	for (auto& vertex : m_vertices)
	{
		vertex.color = m_rgba;
	}
}

void ModelClass::SetRGB(float r, float g, float b)
{
	SetColor(r, g, b, m_rgba.w);
}

void ModelClass::SetAlpha(float a)
{
	SetColor(m_rgba.x, m_rgba.y, m_rgba.z, a);
}

void ModelClass::RotationToCamera(Camera& c)
{
	XMFLOAT3 destVec = c.GetVewDir();
	//destVec.x = -destVec.x;
	//destVec.y = -destVec.y;
	//destVec.z = -destVec.z;

	XMFLOAT3 srcVec = { 0.0f, 1.0f, 0.0f };
	auto dot = [](XMFLOAT3 v1, XMFLOAT3 v2)
	{
		return (v1.x * v2.x + v1.y*v2.y + v1.z*v2.z);
	};
	auto rotation = [&](float& x, float &y, float rad)
	{
		x = x * cosf(rad) + y * sinf(rad);
		y = -x * sinf(rad) + y * cosf(rad);
	}; 
	auto vecLen = [](XMFLOAT3 v)
	{
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	};

	auto getAngle = [=](XMFLOAT3 v1, XMFLOAT3 v2)
	{
		return acosf(dot(v1, v2) / vecLen(v1) / vecLen(v2));
	};

	XMFLOAT3 afterRotX = destVec;
	afterRotX.x = 0.0f;

	auto radAxisX = getAngle(srcVec, afterRotX);
	auto radAxisY = getAngle(afterRotX, destVec);

	//시바.. 그냥 하드코딩...
	SetRotation(-0.8f, -3.141592f / 4.0f, 0.0f);
}

