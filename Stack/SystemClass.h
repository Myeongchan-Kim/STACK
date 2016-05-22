#pragma once

#include <windows.h>
class Scene;
class InputClass;
class Renderer;
#include <string>
#include "MyTime.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class SystemClass
{
public:
	static SystemClass* GetInstance();
	~SystemClass();
	void SetScene(Scene* scene);
	bool Initialize();
	void Shutdown();
	void Run();
	void StopSound(std::string fileName);
	void PlaySoundFile(std::string fileName);

	void CloseSoundFile(std::string fileName);
	void PreLoadSoundFile(std::string fileName);
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	SystemClass();
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	static SystemClass* instance;
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_input;
	CMyTime m_timer;
	Renderer* m_renderer;
	Scene* m_currentScene = nullptr;
	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


static SystemClass* ApplicationHandle = nullptr;
