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

float4 PS(VertexOut vOut, uniform int alphaLevel) : SV_TARGET
{
	float4 finalColor = 0;
	float bright = saturate(dot((float4) - lightDir, vOut.normal) * 0.5 + 0.5);

	finalColor = bright * vOut.color;
	finalColor = finalColor - float4(0,0,0, alphaLevel * 0.2f);
	float4 texColor = texDiffuse.Sample(samLinear, vOut.tex);

	float4 result;
	result = 0.5*texColor + 0.5*finalColor;
	result.a = finalColor.a;
	return result;
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
		SetPixelShader(CompileShader(ps_5_0, PS(0)));

	}
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(1)));

	}
	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(2)));

	}
	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(3)));

	}
	pass P4
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(4)));

	}
	pass P5
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(5)));

	}
};