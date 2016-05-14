#include <time.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "Renderer.h"
#include "MyVertex.h"
#include "WICTextureLoader.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}



bool Renderer::Initialize(int winWidth, int winHeight, HWND hwnd)
{
	m_width = winWidth;
	m_height = winHeight;
	InitDevice(hwnd);

	m_modelList.emplace_back(new ModelClass());
	CreateShader();
	for (auto model : m_modelList)
	{
		//model->SetSample();
		model->SetToCube(4, 2, 1);
		assert(model->indexSize() == 36);
		model->CreateVertexBuffer(m_device);
		model->CreateIndexBuffer(m_device);
	}

	CreateConstantBuffer();
	InitMatrix();
	LoadTexture();
	
	return true;
}

HRESULT Renderer::InitDevice(HWND hwnd)
{
	HRESULT hr = S_OK;

	//Flag설정
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;		// 백 버퍼 갯수

	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//백버퍼 포맷
	sd.BufferDesc.RefreshRate.Numerator = 60;	//분자
	sd.BufferDesc.RefreshRate.Denominator = 1;	//분모

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 백버퍼 랜더링
	sd.OutputWindow = hwnd;	//출력할 윈도우 핸들
	sd.SampleDesc.Count = 1;	//multisampling(다중 표본화) 1이면 한가지 색깔로 다중표본화 --> 다중표본화 안하겠다는 뜻.
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapChain,
		&m_device,
		&m_featureLevel,
		&m_immediateContext);

	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_swapChain->GetBuffer(0,							 // 후면 버퍼 인덱스, 여러개일 때 중요, 지금은 1개 이므로 0.
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = m_device->CreateRenderTargetView(pBackBuffer,
		NULL,
		&m_renderTargetView);

	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	//여기에 원래는 depthbuffer의 뷰를 넣어야함.

	D3D11_VIEWPORT	vp; // 한 윈도우에 여러개의 화면을 그릴 수 있는 박스들. ex)레이싱 게임 2player모드.
	vp.Width = m_width;
	vp.Height = m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_immediateContext->RSSetViewports(1, &vp); // 뷰포트가 여러개면 개수와, 배열의 주소를 넣는다.

	CreateDepthStencilTexture();
	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return hr;

}


void Renderer::CreateShader()
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	DWORD shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	HRESULT hr = D3DCompileFromFile(
		L"./MyShader.fx", 0, 0, //shader 파일 설정
		0, "fx_5_0", shaderFlags,
		0, &compiledShader,
		&compilationMsgs
		);

	if (FAILED(hr))
		return;

	hr = D3DX11CreateEffectFromMemory(
		compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_device,
		&m_effect);


	if (FAILED(hr))
		return;

	m_tech = m_effect->GetTechniqueByName("ColorTech");
	m_wvp = m_effect->GetVariableByName("wvp")->AsMatrix();
	m_world = m_effect->GetVariableByName("world")->AsMatrix();
	m_lightDir = m_effect->GetVariableByName("lightDir")->AsVector();
	m_lightColor = m_effect->GetVariableByName("lightColor")->AsVector();
	m_texDiffuse = m_effect->GetVariableByName("texDiffuse")->AsShaderResource();
	m_samLinear = m_effect->GetVariableByName("samLinear")->AsSampler();

	if (FAILED(hr))
		return;


	D3D11_INPUT_ELEMENT_DESC	layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	UINT numElements = ARRAYSIZE(layout);

	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);

	hr = m_device->CreateInputLayout(
		layout,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, 
		&m_inputLayout);

	if (FAILED(hr))
		return;
}


void Renderer::InitMatrix()
{
	
	// View 행렬 구성
	XMVECTOR 	pos = XMVectorSet(-10.0f, 15.0f, -10.0f, 1.0f);
	XMVECTOR 	target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR 	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(pos, target, up);

	// Projection 행렬
	m_projection = XMMatrixOrthographicLH((float)m_width/100, (float)m_height/100, 0.1f, 1000.0f);  	// near plane, far plane

}

void   Renderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC 	cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	m_device->CreateBuffer(&cbd, NULL, &m_constantBuffer);
}

void Renderer::CalculateMatrixForBox(float deltaTime, ModelClass* model)
{
	// 박스를 회전시키기 위한 연산.    위치, 크기를 변경하고자 한다면 SRT를 기억할 것.      

	XMFLOAT3 rot = model->GetRotation(); 
	XMFLOAT3 pos = model->GetPosition();

	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX rotation = XMMatrixRotationX(rot.x) *XMMatrixRotationY(rot.y) *XMMatrixRotationZ(rot.z);
	XMMATRIX trans = XMMatrixTranslation(pos.x, pos.y, pos.z);

	XMMATRIX world = scale * rotation * trans;
	XMMATRIX wvp = world * m_view * m_projection;

	m_wvp->SetMatrix((float*)&wvp);
	m_world->SetMatrix((float*)&world);
}

void Renderer::CalculateMatrixForBox2(float deltaTime)
{
	float scaleValue = sinf(deltaTime) * 0.5f + 1;
	XMMATRIX scale = XMMatrixScaling(scaleValue, scaleValue, scaleValue);
	XMMATRIX rotate = XMMatrixRotationZ(deltaTime);
	float moveValue = cosf(deltaTime) * 5.0f;
	XMMATRIX position = XMMatrixTranslation(moveValue, 0.0f, 0.0f);
	m_world2 = scale * rotate * position;

	XMMATRIX wvp = m_world2 * m_view * m_projection;
	ConstantBuffer cb;
	cb.wvp = XMMatrixTranspose(wvp);
	cb.world = XMMatrixTranspose(m_world2);
	cb.lightDir = lightDirection;
	cb.lightColor = lightColor;

	m_immediateContext->UpdateSubresource(m_constantBuffer, 0, 0, &cb, 0, 0);
	m_immediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}


void Renderer::CreateDepthStencilTexture()
{
	//Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1; //밉맵은 쓰지 않는다.
	descDepth.ArraySize = 1; //따라서 하나의 스탠실 버퍼 사용
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_device->CreateTexture2D(&descDepth, NULL, &m_depthStencil);

	//Create depth stencil view

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;

	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	m_device->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);


}

HRESULT Renderer::LoadTexture()
{

	
	HRESULT hr = CreateWICTextureFromFile(
		m_device,
		m_immediateContext,
		L"Texture/images.jpg",
		nullptr, //여기에 texture넣기.
		&m_textureRV
		);

	D3D11_SAMPLER_DESC	sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&sampDesc, &m_samplerLinear);


	return hr;

}

bool Renderer::Frame(float deltaTime)
{
	//model position 계산
	for (auto model : m_modelList)
	{
		model->SetPosition(sinf(deltaTime * 3), 0.0f, cosf(deltaTime * 3));
		model->SetRotation(sinf(deltaTime), cosf(deltaTime *2), sinf(deltaTime *0.5));
	}

	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, ClearColor);
	m_immediateContext->ClearDepthStencilView(
		m_depthStencilView,		//clear target
		D3D11_CLEAR_DEPTH,      //clear flag(depth, stencil)
		1.0f,					//depth buffer 지울 때 채울값
		0);						//stencil buffer 지울 때 초기값   

	m_immediateContext->IASetInputLayout(m_inputLayout);
	m_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto model : m_modelList)
	{
		UINT stride = sizeof(MyVertex);
		UINT offset = 0;
		m_immediateContext->IASetVertexBuffers(0, 1, &model->GetVB(), &stride, &offset);
		m_immediateContext->IASetIndexBuffer(model->GetIB(), DXGI_FORMAT_R16_UINT, 0);

		m_texDiffuse->SetResource(m_textureRV);
		m_samLinear->SetSampler(0, m_samplerLinear);
		// 계산 및 그리기
		CalculateMatrixForBox(deltaTime, model);

		//빛 계산
		m_lightDir->SetFloatVector((float*)&lightDirection);
		m_lightColor->SetFloatVector((float*)&lightColor);

		D3DX11_TECHNIQUE_DESC techDesc;
		m_tech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_tech->GetPassByIndex(p)->Apply(0, m_immediateContext);

			m_immediateContext->DrawIndexed(model->indexSize(), 0, 0);
		}
	}
	
	m_swapChain->Present(0, 0);

	return true;
}


void Renderer::CreateRenderState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;

	m_device->CreateRasterizerState(&rasterizerDesc, &m_solidRS);

}

void Renderer::CreateRenderState2()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;

	m_device->CreateRasterizerState(&rasterizerDesc, &m_wireFrameRS);

}

void Renderer::ShutDown()
{
	for (auto model : m_modelList)
	{
		model->ReleaseVB();
		model->ReleaseIB();
		if (!model)
		{
			delete model;
			model = nullptr;
		}
		
	}
	if (m_immediateContext) m_immediateContext->ClearState();

	if (m_constantBuffer) m_constantBuffer->Release();
	if (m_inputLayout) m_inputLayout->Release();
	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_pixelShader->Release();

	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_device) m_device->Release();


	if (m_depthStencil) m_depthStencil->Release();
	if (m_depthStencilView) m_depthStencilView->Release();
	if (m_vertexShader) m_vertexShader->Release();
	if (m_inputLayout) m_inputLayout->Release();
	if (m_pixelShader) m_pixelShader->Release();
	if (m_solidRS) m_solidRS->Release();
	if (m_wireFrameRS) m_wireFrameRS->Release();
	if (m_textureRV) m_textureRV->Release();
	if (m_samplerLinear) m_samplerLinear->Release();
	if (m_effect) m_effect->Release();


}
