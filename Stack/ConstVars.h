#pragma once
#include <d3d11.h>
#include <string>

#define KILLME false
#define DONTKILL true

namespace ConstVars
{
	extern const int WINDOW_WIDTH;
	extern const int WINDOW_HEIGHT;
	extern const float DEFAULT_VIEW_WIDTH;
	extern const float DEFAULT_VIEW_HEIGHT;
	extern WCHAR* CONCREAT_TEX_FILE;
	extern WCHAR* FEBRIC_TEX_FILE;
	extern WCHAR* PLANE_TEX_FILE;
	extern WCHAR* BLUE_TILE_TEX_FILE;
	extern WCHAR* COLORFUL_TILE_TEX_FILE;
	
	extern WCHAR* VISIBLE_TEX_FILE;
	extern WCHAR* VANISHING_TEX_FILE;
	
	extern WCHAR* MAIN_MENU_TEX_FILE;
	extern WCHAR* CHAR_TEX_FILE;

	extern WCHAR* LOG_FILENAME;

	extern std::string SOUND_FILE1;
	extern std::string SOUND_FILE2;
	extern std::string SOUND_FILE3;
	extern std::string SOUND_FILE4;
	extern std::string SOUND_FILE5;
	extern std::string SOUND_FILE6;
	extern std::string SOUND_FILE7;
	extern std::string SOUND_FILE8;
}	
