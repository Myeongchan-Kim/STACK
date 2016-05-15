#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

class MyVertex;
class ModelClass
{
public:
	ModelClass(float x, float y, float z);
	~ModelClass();
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
	ID3D11Buffer*& GetVB() { return m_vertexBuffer; };
	ID3D11Buffer*& GetIB() { return m_indexBuffer; };
	void ReleaseVB();
	void ReleaseIB();
	UINT indexSize() { return m_indices.size(); };

	void SetSample();
	void SetToCube(float widthX, float height, float widthZ);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	WCHAR*& GetTextureName() { return m_textureName; };
	void SetTextureName(WCHAR* textureName);

private:
	void AddRectangle(MyVertex& v1, MyVertex& v2, MyVertex& v3, MyVertex& v4);

	WCHAR* m_textureName;

	ID3D11Buffer*		m_vertexBuffer = nullptr;
	ID3D11Buffer*		m_indexBuffer = nullptr;
	std::vector<MyVertex>	m_vertices;
	std::vector<WORD>	m_indices;

	XMFLOAT3			m_pos = {0.0f, 0.0f, 0.0f};
	
	float				m_xRot = 0.0f;
	float				m_yRot = 0.0f;
	float				m_zRot = 0.0f;
};

