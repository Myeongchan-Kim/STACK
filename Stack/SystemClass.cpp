#include "SystemClass.h"

SystemClass::SystemClass()
{

	m_Input = nullptr;
	m_renderer = nullptr;
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;
	m_timer.Init();


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass();
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_renderer = new Renderer();
	if (!m_renderer)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_renderer->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}


/*
	The Shutdown function does the clean up.
	It shuts down and releases everything associated with the graphics and input object.
	As well it also shuts down the window and cleans up the handles associated with it.
*/
void SystemClass::Shutdown()
{
	// Release the graphics object.
	if (m_renderer)
	{
		m_renderer->ShutDown();
		delete m_renderer;
		m_renderer = nullptr;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}
/*
The Run function is where our application will loop and do all the application processing until we decide to quit.
The application processing is done in the Frame function which is called each loop.This is an important concept to understand as now the rest of our application must be written with this in mind.
*/

void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}
/*
	The following Frame function is where all the processing for our application is done.
	So far it is fairly simple, we check the input object to see if the user has pressed escape and wants to quit.
	If they don't want to quit then we call the graphics object to do its frame processing which will render the graphics for that frame.
	As the application grows we'll place more code inside here.
*/

bool SystemClass::Frame()
{
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	m_timer.ProcessTime();
	static float totalElapsedTime = 0;
	totalElapsedTime += m_timer.GetElapsedTime();

	static float count = 0;
	if (m_Input->IsKeyDown(VK_SPACE))
	{
		ModelClass* model = new ModelClass(0, count, 0);
		model->SetToCube(2, 1, 2);
		m_renderer->AddModel(model);
		count += 1.0;
	}
	m_Input->KeyUp(VK_SPACE);
	result = m_renderer->Frame(totalElapsedTime);
	
	return result;
}
/*
	The MessageHandler function is where we direct the windows system messages into.
	This way we can listen for certain information that we are interested in.
	Currently we will just read if a key is pressed or if a key is released and pass that information on to the input object.
	All other information we will pass back to the windows default message handler.
*/

/*
	wParam, lParam은 iMessage의 메시지에 따른 부가적인 정보를 가진다.
	예를 들어 마우스 버튼이 눌러졌다는 WM_LBUTTONDOWN 메시지가 입력되었다면 화면의 어디쯤에서 마우스 버튼이 눌러졌는가,
	그때의 키보드 상황(Shift, Ctrl, Alt)은 어떠한가에 관한 정보가 추가로 필요하며 WM_CHAR 메시지,
	즉 키보드로부터 키가 입력되었다는 메시지가 입력되었다면 어떤 키가 입력되었는가에 관한 추가적인 정보가 필요하다.
	이런 추가 정보들이 wParam, lParam으로 전달된다. wParam, lParam에 실제로 어떤 값을 전달되는지는 메시지별로 다르다.
*/

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}


/*
	The InitializeWindows function is where we put the code to build the window we will use to render to.
	It returns screenWidth and screenHeight back to the calling function so we can make use of them throughout the application.
	We create the window using some default settings to initialize a plain black window with no borders.
	The function will make either a small window or make a full screen window depending on a global variable called FULL_SCREEN.
	If this is set to true then we make the screen cover the entire users desktop window.If it is set to false we just make a 800x600 window in the middle of the screen.
	I placed the FULL_SCREEN global variable at the top of the graphicsclass.h file in case you want to modify it.It will make sense later why I placed the global in that file instead of the header for this file.
*/
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	int windowstate;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
		windowstate = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		windowstate = WS_OVERLAPPEDWINDOW;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
		m_applicationName, 
		m_applicationName,
		windowstate,
		posX, 
		posY, 
		screenWidth, 
		screenHeight, 
		NULL, 
		NULL, 
		m_hinstance, 
		NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}


/*
	ShutdownWindows does just that.It returns the screen settings back to normal and releases the window and the handles associated with it.
*/
void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}