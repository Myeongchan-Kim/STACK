#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class MyVertex;
class ModelClass
{
public:
	ModelClass();
	~ModelClass();
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
	ID3D11Buffer*& GetVB() { return m_vertexBuffer; };
	ID3D11Buffer*& GetIB() { return m_indexBuffer; };
	void ReleaseVB();
	void ReleaseIB();
	UINT indexSize() { return m_indices.size(); };

	void SetSample();
	//void SetToCube(float widthX, float widthZ, float height);
	void SetPosition(float x, float y, float z);
	DirectX::XMFLOAT3 GetPosition();

private:
	ID3D11Buffer*		m_vertexBuffer = nullptr;
	ID3D11Buffer*		m_indexBuffer = nullptr;
	std::vector<MyVertex>	m_vertices;
	std::vector<WORD>	m_indices;
	float				m_xPos = 0.0f;
	float				m_yPos = 0.0f;
	float				m_zPos = 0.0f;
};

