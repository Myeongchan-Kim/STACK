#include "SystemClass.h"
#include "Scene.h"
#include "GameScene.h"
#include "ConstVars.h"

void LoadFiles()
{
	SystemClass::GetInstance()->LoadTexture(ConstVars::CONCREAT_TEX_FILE);
	SystemClass::GetInstance()->LoadTexture(ConstVars::FEBRIC_TEX_FILE);
	SystemClass::GetInstance()->LoadTexture(ConstVars::PLANE_TEX_FILE);
	SystemClass::GetInstance()->LoadTexture(ConstVars::BLUE_TILE_TEX_FILE);
	SystemClass::GetInstance()->LoadTexture(ConstVars::COLORFUL_TILE_TEX_FILE);
	SystemClass::GetInstance()->LoadTexture(ConstVars::MAIN_MENU_TEX_FILE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object.
	System = SystemClass::GetInstance();
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	Scene* scene = new GameScene();
	System->SetScene(scene);
	LoadFiles();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
