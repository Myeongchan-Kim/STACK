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
	float bright = saturate(dot((float4) - lightDir, vOut.normal) * 0.5 + 0.5);

	finalColor = bright * vOut.color;

	float4 texColor = texDiffuse.Sample(samLinear, vOut.tex);
	float alpha = 0.5f;

	return (texColor*alpha) + (1-alpha)*finalColor;
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

		//SetRasterizerState(WireframeRS);
	}
};

