Texture2D texDiffuse;	//texture
SamplerState samLinear;	//sample State(filtering/mode)

cbuffer ConstantBuffer
{
	float4x4 	wvp;
	float4x4	world;

	float4 lightDir;
	float4 lightColor;
}

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;

	float2 tex: TEXCOORD;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;

	float4 normal : NORMAL;
	float2 tex: TEXCOORD;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;

	vOut.pos = mul(float4(vIn.pos, 1.0f), wvp);
	vOut.normal = mul(float4(vIn.normal, 0.0f), world);
	vOut.color = vIn.color;
	vOut.tex = vIn.tex;

	return vOut;
};

float4 PS(VertexOut vOut) : SV_TARGET
{
	float4 finalColor = 0;
	float bright = saturate(dot((float4) - lightDir, vOut.normal) * 0.7 + 0.3);

	finalColor = bright * vOut.color;
	finalColor.a = vOut.color.a;
	float4 texColor = texDiffuse.Sample(samLinear, vOut.tex);

	float4 result;
	result = 0.3*texColor + 0.7*finalColor;
	result.a = finalColor.a;
	return result;
};

float4 TransparentPS(VertexOut vOut) : SV_TARGET
{
	float4 finalColor = 0;
	float bright = saturate(dot((float4) - lightDir, vOut.normal) * 0.7 + 0.3);

	finalColor = bright * vOut.color;
	finalColor.a = vOut.color.a;
	float4 texColor = texDiffuse.Sample(samLinear, vOut.tex);

	float4 result;
	result = 0.2*texColor + 0.8*finalColor;
	result.a = finalColor.a;
	return result;
};

VertexOut UiVS(VertexIn vIn)
{
	VertexOut vOut;

	vOut.pos = float4(vIn.pos, 1.0f);
	vOut.normal = float4(vIn.normal, 0.0f);
	vOut.color = vIn.color;
	vOut.tex = vIn.tex;

	return vOut;
};

float4 UiPS(VertexOut vOut) : SV_TARGET
{
	float4 texColor = texDiffuse.Sample(samLinear, vOut.tex);
	return texColor;
};

RasterizerState WireframeRS
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));

	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, TransparentPS()));
	}

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, UiVS()));
		SetPixelShader(CompileShader(ps_5_0, UiPS()));
	}
};