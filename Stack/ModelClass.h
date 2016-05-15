#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Object.h"
using namespace DirectX;

class MyVertex;
class ModelClass :public Object
{
public:
	ModelClass(float x, float y, float z);
	virtual ~ModelClass();
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
	ID3D11Buffer*& GetVB() { return m_vertexBuffer; };
	ID3D11Buffer*& GetIB() { return m_indexBuffer; };
	void ReleaseVB();
	void ReleaseIB();
	UINT indexSize() { return m_indices.size(); };

	void SetSample();
	void SetToCube(float widthX, float height, float widthZ);
	void SetToRectangle(float width, float height, XMFLOAT3 normal);

	void SetPosition(float x, float y, float z);
	void SetColor(float r, float g, float b, float a);
	void SetRGB(float r, float g, float b);
	void SetAlpha(float a);
	void SetRotation(float x, float y, float z);


	void UpTransparency() { if(m_transparency < 5) m_transparency++; };
	DirectX::XMFLOAT3 GetRotation();
	int GetTransparency() { return m_transparency; };
	inline DirectX::XMFLOAT4 GetColor() { return m_rgba; };

	WCHAR*& GetTextureName() { return m_textureName; };
	void SetTextureName(WCHAR* textureName);

private:
	void AddRectangle(MyVertex& v1, MyVertex& v2, MyVertex& v3, MyVertex& v4);

	WCHAR* m_textureName;

	ID3D11Buffer*		m_vertexBuffer = nullptr;
	ID3D11Buffer*		m_indexBuffer = nullptr;
	std::vector<MyVertex>	m_vertices;
	std::vector<WORD>	m_indices;

	DirectX::XMFLOAT4			m_rgba;
	float				m_xRot;
	float				m_yRot;
	float				m_zRot;
	int					m_transparency = 0;
};
