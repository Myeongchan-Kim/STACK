#pragma once
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class ModelClass
{

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
/*
	The functions here handle initializing and shutdown of the model's vertex and index buffers.
	The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
*/
		bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

/*
	Here is the definition of our vertex type that will be used with the vertex buffer in this ModelClass.
	Also take note that this typedef must match the layout in the ColorShaderClass that will be looked at later in the tutorial.
*/
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
/*
	The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer.
	Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description when they are first created.
*/
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
