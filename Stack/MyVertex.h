#pragma once
#include <DirectXmath.h>
using namespace DirectX;

struct MyVertex
{
	XMFLOAT3	pos;
	XMFLOAT4	color;

	XMFLOAT3	normal;
	XMFLOAT2	tex;

	bool operator==(const MyVertex& rv);
};