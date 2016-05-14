#include <dxgi.h>
#include <d3dcompiler.h>
#include <algorithm>
#include "ModelClass.h"
#include "MyVertex.h"
#include "WICTextureLoader.h"


ModelClass::ModelClass(float x, float y, float z) :m_xPos(0.0f), m_yPos(0.0f), m_zPos(0.0f), 
													m_xRot(0.0f), m_yRot(0.0f), m_zRot(0.0f),
													m_rgba( 0.8f, 0.8f, 0.8f, 1.0f ),	m_indexBuffer(nullptr), m_vertexBuffer(nullptr)
{
	SetPosition(x, y, z);								  
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
	return device->CreateBuffer(&ibd,				//생성할 버퍼의 정보를 담은 구조체
		&initData,									//버퍼 초기화시 필요한 데이터
		&m_indexBuffer);							//생성된 버퍼
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

void ModelClass::SetToCube(float widthX, float height, float widthZ)
{
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

XMFLOAT3 ModelClass::GetPosition()
{
	return DirectX::XMFLOAT3({m_xPos, m_yPos, m_zPos});
}

DirectX::XMFLOAT3 ModelClass::GetRotation()
{
	return DirectX::XMFLOAT3({m_xRot, m_yRot, m_zRot});
}

void ModelClass::SetTextureName(WCHAR * textureName)
{
	m_textureName = textureName;
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;
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

void ModelClass::SetRotation(float x, float y, float z)
{
	m_xRot = x;
	m_yRot = y;
	m_zRot = z;
}
