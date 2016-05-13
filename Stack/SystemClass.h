#pragma once

#include <windows.h>
#include "InputClass.h"
#include "Renderer.h"
#include "MyTime.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class SystemClass
{
public:

	SystemClass();
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	Renderer* m_Graphics;
	CMyTime m_timer;
	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


static SystemClass* ApplicationHandle = nullptr;
