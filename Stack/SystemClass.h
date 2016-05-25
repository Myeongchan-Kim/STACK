#pragma once

#include <windows.h>
#include "ModelClass.h"
class Scene;
class InputClass;
class Renderer;
#include <string>
#include "MyTime.h"
#include <unordered_map>

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
	void LoadTexture(WCHAR* fileName);
	void SetLightDirection(XMFLOAT4 lightDirection);
	void CloseSoundFile(std::string fileName);
	void PreLoadSoundFile(std::string fileName);
	void StoreIntVariable(std::string name, int value);
	bool HasIntVariable(std::string name);
	int GetIntVariable(std::string name);
	void SetIntVariable(std::string name, int value);
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

	std::unordered_map<std::string, int> m_intStorage;
	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


static SystemClass* ApplicationHandle = nullptr;
