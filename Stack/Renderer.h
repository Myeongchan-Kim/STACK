#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "Common\d3dx11effect.h"
using namespace DirectX;


class Renderer
{
public:
	Renderer();
	~Renderer();
	bool Initialize(int winWidth, int winHeight, HWND hwnd);
	bool Frame(float deltaTime);
	void ShutDown();

private:


	HRESULT InitDevice(HWND hwnd);
	void CreateShader();
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	void InitMatrix();
	void CreateConstantBuffer();
	void CalculateMatrixForBox(float deltaTime);
	void CalculateMatrixForBox2(float deltaTime);
	void CreateDepthStencilTexture();
	HRESULT LoadTexture();
	void CreateRenderState();
	void CreateRenderState2();

	struct MyVertex
	{
		XMFLOAT3	pos;
		XMFLOAT4	color;

		XMFLOAT3	normal;
		XMFLOAT2	tex;
	};

	XMFLOAT4 lightDirection =
	{
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
	};

	XMFLOAT4 lightColor =
	{
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	IDXGISwapChain*					m_swapChain = nullptr; //DC 바꾸기
	ID3D11Device*					m_device = nullptr;
	ID3D11DeviceContext*			m_immediateContext = nullptr; //Dx용 DC
	ID3D11RenderTargetView*			m_renderTargetView = nullptr;
	D3D_FEATURE_LEVEL				m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D*				m_depthStencil = nullptr;
	ID3D11DepthStencilView*			m_depthStencilView = nullptr;

	ID3D11VertexShader*				m_vertexShader = nullptr;
	ID3D11InputLayout*				m_inputLayout = nullptr;
	ID3D11PixelShader*				m_pixelShader = nullptr;
	ID3D11Buffer*					m_constantBuffer = nullptr;
	ID3D11Buffer*					m_vertexBuffer = nullptr;
	ID3D11Buffer*					m_indexBuffer = nullptr;

	ID3D11RasterizerState*			m_solidRS = nullptr;
	ID3D11RasterizerState*			m_wireFrameRS = nullptr;

	ID3D11ShaderResourceView*		m_textureRV = nullptr;
	ID3D11Resource*					m_texture = nullptr;
	ID3D11SamplerState*				m_samplerLinear = nullptr;
	ID3DX11Effect*					m_effect = nullptr;
	ID3DX11EffectTechnique*			m_tech = nullptr;

	XMMATRIX						m_world;
	XMMATRIX						m_world2;
	XMMATRIX						m_view;
	XMMATRIX						m_projection;

	int								m_width;
	int								m_height;


	struct	 ConstantBuffer
	{
		XMMATRIX wvp;
		XMMATRIX world;

		XMFLOAT4 lightDir;
		XMFLOAT4 lightColor;
	};

};

