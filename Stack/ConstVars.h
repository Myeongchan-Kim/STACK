#pragma once
#include <d3d11.h>

#define KILLME true
#define DONTKILL false

namespace ConstVars
{
	static WCHAR* CONCREAT_TEX_FILE = L"texture/concrete.jpg";
	static WCHAR* FEBRIC_TEX_FILE = L"texture/febric.jpg";
	static WCHAR* PLANE_TEX_FILE = L"texture/plane.jpg";
	static WCHAR* BLUE_TILE_TEX_FILE = L"texture/blue_tile.jpg";
	static WCHAR* COLORFUL_TILE_TEX_FILE = L"texture/colorful_tile.jpg";
	
	//¼³Á¤
	static WCHAR* VISIBLE_TEX_FILE = COLORFUL_TILE_TEX_FILE;
	static WCHAR* VANISHING_TEX_FILE = BLUE_TILE_TEX_FILE;

}
