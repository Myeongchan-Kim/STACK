#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <map>
#include <memory>
#include "ModelClass.h"
#include "Common\d3dx11effect.h"
#include "Camera.h"

using namespace DirectX;

class Renderer
{
public:
	Renderer();
	~Renderer();
	bool Initialize(int winWidth, int winHeight, HWND hwnd);
	void AddModel(ModelClass* model);
	void AddTransparentModel(ModelClass* model);
	bool Frame(float deltaTime);
	void ShutDown();
	void SetCameraPos(float x, float y, float z) { m_camera.MoveBy(x, y, z); };
	void MoveCameraFor(float x, float y, float z, float time);

private:


	HRESULT InitDevice(HWND hwnd);
	void CreateShader();
	void CalculateMatrixForBox(float deltaTime, ModelClass* model);
	void CreateDepthStencilState();
	void CreateDepthStencilTexture();
	HRESULT CreateBlendState();
	HRESULT LoadTexture(WCHAR* fileName);
	void CreateRenderState();

	XMFLOAT4 lightDirection = { 0.0f, -1.0f, -0.3f, 1.0f };
	XMFLOAT4 lightColor = {1.0f, 1.0f, 1.0f, 1.0f};
	Camera				m_camera;
	int					m_width;
	int					m_height;

	IDXGISwapChain*							m_swapChain = nullptr; //DC 바꾸기
	ID3D11Device*							m_device = nullptr;
	ID3D11DeviceContext*					m_immediateContext = nullptr; //Dx용 DC
	ID3D11RenderTargetView*					m_renderTargetView = nullptr;

	D3D_FEATURE_LEVEL						m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D*						m_depthStencil = nullptr;
	ID3D11DepthStencilView*					m_depthStencilView = nullptr;
	ID3D11DepthStencilState*				m_depthStencilStateZTestOn = nullptr;
	ID3D11DepthStencilState*				m_depthStencilStateZTestOff = nullptr;
	ID3D11BlendState*						m_blendState = nullptr;


	ID3D11VertexShader*						m_vertexShader = nullptr;
	ID3D11InputLayout*						m_inputLayout = nullptr;
	ID3D11PixelShader*						m_pixelShader = nullptr;
	ID3D11Buffer*							m_constantBuffer = nullptr;
	
	ID3D11RasterizerState*					m_solidRS = nullptr;
	ID3D11RasterizerState*					m_wireFrameRS = nullptr;

	ID3DX11Effect*							m_effect = nullptr;
	ID3DX11EffectTechnique*					m_colorTech = nullptr;
	std::vector<ID3DX11EffectTechnique*>	m_transparentTechList;

	ID3DX11EffectSamplerVariable*			m_samLinear = nullptr;
	ID3DX11EffectShaderResourceVariable*	m_texDiffuse = nullptr;
	ID3DX11EffectMatrixVariable*			m_wvp;
	ID3DX11EffectMatrixVariable*			m_world;
	ID3DX11EffectVectorVariable*			m_lightDir;
	ID3DX11EffectVectorVariable*			m_lightColor;

	ID3D11Resource*							m_texture = nullptr;
	ID3D11SamplerState*						m_samplerLinear = nullptr;

	std::map<WCHAR*, ID3D11ShaderResourceView*>	m_textureRVList;
	std::vector<ModelClass*>				m_modelList;
	std::vector<ModelClass*>				m_transparentModelList;

	struct	 ConstantBuffer
	{
		XMMATRIX wvp;
		XMMATRIX world;

		XMFLOAT4 lightDir;
		XMFLOAT4 lightColor;
	};

};

