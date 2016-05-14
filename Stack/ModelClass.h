#pragma once
#include <d3d11.h>
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

	void SetSampleVertices();

private:
	ID3D11Buffer*		m_vertexBuffer = nullptr;
	ID3D11Buffer*		m_indexBuffer = nullptr;
	std::vector<MyVertex>	m_vertices;
};

