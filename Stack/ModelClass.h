#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Camera.h"
#include "Object.h"
using namespace DirectX;
struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector2 {
	float x;
	float y;
};


class MyVertex;
class ModelClass :public Object
{
public:
	ModelClass();
	virtual ~ModelClass();
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
	ID3D11Buffer*& GetVB() { return m_vertexBuffer; };
	ID3D11Buffer*& GetIB() { return m_indexBuffer; };
	int GetTransparency() { return m_transparency; };
	void SetTransParency(float alpha);

	void ReleaseVB();
	void ReleaseIB();
	UINT indexSize() { return m_indices.size(); };

	void SetSample();
	void SetToCube(XMFLOAT3 boxSize);
	void SetToRectangle(float width, float height, XMFLOAT3 normal);
	void SetTextureName(WCHAR* textureName);
	void SetColor(float r, float g, float b, float a);
	void SetRGB(float r, float g, float b);
	void SetAlpha(float a);
	void RotationToCamera(Camera& c);

	DirectX::XMFLOAT3 GetRotation();
	inline DirectX::XMFLOAT4 GetColor() { return m_rgba; };
	WCHAR*& GetTextureName() { return m_textureName; };

protected:
	void AddRectangle(MyVertex& v1, MyVertex& v2, MyVertex& v3, MyVertex& v4);
	bool m_vertexIsChanged = true;

	WCHAR*						m_textureName;

	ID3D11Buffer*				m_vertexBuffer = nullptr;
	ID3D11Buffer*				m_indexBuffer = nullptr;
	std::vector<MyVertex>		m_vertices;
	std::vector<WORD>			m_indices;
	int							m_transparency = 0;

};
