#include <dxgi.h>
#include <d3dcompiler.h>
#include "ModelClass.h"
#include "MyVertex.h"
#include "WICTextureLoader.h"


ModelClass::ModelClass() :m_indexBuffer(nullptr), m_vertexBuffer(nullptr)
{
}


ModelClass::~ModelClass()
{
}


HRESULT ModelClass::CreateVertexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = m_vertices.size() * sizeof(MyVertex);			//���� ũ��
	bd.Usage = D3D11_USAGE_DEFAULT;				//���� ��� ���
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���������ο� ����Ǵ� ���� ����
	bd.CPUAccessFlags = 0;						//cpu���� flag. �Ϲ������� GPU�� ����ϱ� ������ 0�� ����.

	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_vertices[0];				//�ʱ�ȭ�ϱ� ���� ���� �迭 ������

	return device->CreateBuffer(&bd,			//������ ������ ������ ���� ����ü
		&initData,								//���� �ʱ�ȭ�� �ʿ��� ������
		&m_vertexBuffer);						//������ ����
}


HRESULT ModelClass::CreateIndexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.ByteWidth = m_indices.size() * sizeof(WORD);				//���� ũ��
	ibd.Usage = D3D11_USAGE_IMMUTABLE;				//���� ��� ���
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//���������ο� ����Ǵ� ���� ����
	ibd.CPUAccessFlags = 0;							//cpu���� flag. �Ϲ������� GPU�� ����ϱ� ������ 0�� ����.

	D3D11_SUBRESOURCE_DATA	initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &m_indices[0];						//�ʱ�ȭ�ϱ� ���� ���� �迭 ������
	return device->CreateBuffer(&ibd,				//������ ������ ������ ���� ����ü
		&initData,									//���� �ʱ�ȭ�� �ʿ��� ������
		&m_indexBuffer);							//������ ����
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

XMFLOAT3 ModelClass::GetPosition()
{
	return XMFLOAT3({m_xPos, m_yPos, m_zPos});
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;
}
