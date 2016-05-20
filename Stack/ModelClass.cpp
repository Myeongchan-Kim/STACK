#include <dxgi.h>
#include <d3dcompiler.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include "ModelClass.h"
#include "MyVertex.h"
#include "WICTextureLoader.h"
#include "ConstVars.h"

ModelClass::ModelClass() : m_indexBuffer(nullptr), m_vertexBuffer(nullptr), m_textureName(ConstVars::PLANE_TEX_FILE)
{

}

ModelClass::ModelClass(std::string filename) : m_indexBuffer(nullptr), m_vertexBuffer(nullptr), m_textureName(ConstVars::PLANE_TEX_FILE)

{
	std::ifstream fin;
	char input;

	struct face {

		int ver1;
		int tex1;
		int nor1;
		int ver2;
		int tex2;
		int nor2;
		int ver3;
		int tex3;
		int nor3;
	};

	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<face> faces;

	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return;
	}
	while (!fin.eof())
	{
		char input;
		fin.get(input);
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				float x;
				float y;
				float z;
				fin >> x >> y >> z;

				vertices.push_back(Vector3{ x,y,z });
				vertexCount++;
			}
			else if (input == 't')
			{
				float x;
				float y;
				fin >> x >> y;
				texcoords.push_back(Vector2{ x,y });
				textureCount++;
			}
			else if (input == 'n')
			{
				float x;
				float y;
				float z;
				fin >> x >> y >> z;

				normals.push_back(Vector3{ x,y,z });
				normalCount++;
			}
		}
		else if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				char input1, input2;

				int v1, v2, v3;
				int t1, t2, t3;
				int n1, n2, n3;

				fin >> v1 >> input1 >> t1 >> input2 >> n1
					>> v2 >> input1 >> t2 >> input2 >> n2
					>> v3 >> input1 >> t3 >> input2 >> n3;

				faces.push_back(face{
				v1, t1, n1,
				v2, t2, n2,
				v3, t3, n3
				});
				faceCount++;
			}
		}
	}
	fin.close();

	for (int i = 0 ; i < faceCount; i++)
	{
		XMFLOAT3	pos1 = 
		{
			vertices[faces[i].ver1-1].x,
			vertices[faces[i].ver1-1].y,
			vertices[faces[i].ver1-1].z,
		};

		XMFLOAT4	color1 =
		{
			1,1,1,1
		};

		XMFLOAT3	normal1 =
		{
			normals[faces[i].nor1-1].x,
			normals[faces[i].nor1-1].y,
			normals[faces[i].nor1-1].z
		};

		XMFLOAT2	tex1 =
		{
			texcoords[faces[i].tex1-1].x,
			texcoords[faces[i].tex1-1].y
		};

		XMFLOAT3	pos2 =
		{
			vertices[faces[i].ver2-1].x,
			vertices[faces[i].ver2-1].y,
			vertices[faces[i].ver2-1].z,
		};

		XMFLOAT4	color2 =
		{
			1,1,1,1
		};

		XMFLOAT3	normal2 =
		{
			normals[faces[i].nor2-1].x,
			normals[faces[i].nor2-1].y,
			normals[faces[i].nor2-1].z
		};

		XMFLOAT2	tex2 =
		{
			texcoords[faces[i].tex2-1].x,
			texcoords[faces[i].tex2-1].y
		};

		XMFLOAT3	pos3 =
		{
			vertices[faces[i].ver3-1].x,
			vertices[faces[i].ver3-1].y,
			vertices[faces[i].ver3-1].z,
		};

		XMFLOAT4	color3 =
		{
			1,1,1,1
		};

		XMFLOAT3	normal3 =
		{
			normals[faces[i].nor3-1].x,
			normals[faces[i].nor3-1].y,
			normals[faces[i].nor3-1].z
		};

		XMFLOAT2	tex3 =
		{
			texcoords[faces[i].tex3-1].x,
			texcoords[faces[i].tex3-1].y
		};

		MyVertex vertex1
		{
			pos1,
			color1,
			normal1,
			tex1
		};

		MyVertex vertex2
		{
			pos2,
			color2,
			normal2,
			tex2
		};

		MyVertex vertex3
		{
			pos3,
			color3,
			normal3,
			tex3
		};

		m_vertices.push_back(vertex1);
		m_vertices.push_back(vertex2);
		m_vertices.push_back(vertex3);

		m_indices.push_back(i*3);
		m_indices.push_back(i*3+1);
		m_indices.push_back(i*3+2);
	}
}

ModelClass::~ModelClass()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
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
	if (m_vertices.empty())
	{
		assert("Model is not initailized. no vertices in model.");
	}
	initData.pSysMem = &m_vertices[0];				//�ʱ�ȭ�ϱ� ���� ���� �迭 ������

	if (m_vertexBuffer)
	{
		ReleaseVB();
		m_vertexBuffer == nullptr;
	}

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
	if(m_indexBuffer)
		m_indexBuffer->Release();
	return device->CreateBuffer(&ibd,				//������ ������ ������ ���� ����ü
		&initData,									//���� �ʱ�ȭ�� �ʿ��� ������
		&m_indexBuffer);							//������ ����
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
		m_vertexBuffer == nullptr;
	}
}

void ModelClass::ReleaseIB()
{
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer == nullptr;
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
	m_rgba;

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
	//auto radAxisY = acosf(dot({ normal.x, 0.0f, normal.z }, standardNormal) / sqrt(normal.x * normal.x + normal.z * normal.z)); // Y�������� ȸ����
	//auto radAxisZ = acosf(dot({ normal.x, normal.y, 0.0f }, standardNormal) / sqrt(normal.x * normal.x + normal.y * normal.y)); // Z�������� ȸ����

	MyVertex v1 = { pos[0],{ m_rgba.x - 0.3f,m_rgba.y - 0.3f,m_rgba.z - 0.3f, 1.0f }, normal,{ 0.0f, 0.0f } };
	MyVertex v2 = { pos[1], m_rgba, normal,{ 1.0f, 0.0f } };
	MyVertex v3 = { pos[2],{ m_rgba.x - 0.3f,m_rgba.y - 0.3f,m_rgba.z - 0.3f, 1.0f }, normal,{ 1.0f, 1.0f } };
	MyVertex v4 = { pos[3],{ m_rgba.x - 0.8f,m_rgba.y - 0.8f,m_rgba.z - 0.8f, 1.0f }, normal,{ 0.0f, 1.0f } };

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

	//�ù�.. �׳� �ϵ��ڵ�...
	SetRotation(-3.141592f / 4.0f, -3.141592f / 4.0f, 0.0f);
}

