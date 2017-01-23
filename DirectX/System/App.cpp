#include "../stdafx.h"
#include "App.h"
#include "../GraphicsInterface/Graphics.h"
#include "Input.h"

namespace System
{
	//-----------------------------------------------------------------------------------------------------
	AppPtr App::m_app = nullptr;

	//-----------------------------------------------------------------------------------------------------
	AppPtr& App::instance()
	{
		if (m_app == nullptr)
		{
			m_app = std::make_unique<App>();
		}
		return m_app;
	}

	//-----------------------------------------------------------------------------------------------------
	App::~App()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	bool App::init()
	{
		int screenWidth, screenHeight;
		bool result;


		// Initialize the width and height of the screen to zero before sending the variables into the function.
		screenWidth = 0;
		screenHeight = 0;

		// Initialize the windows api.
		initializeWindows(screenWidth, screenHeight);

		// Create the input object.  This object will be used to handle reading the keyboard input from the user.
		m_input = std::make_unique<Input>();
		if (!m_input)
		{
			return false;
		}

		// Initialize the input object.
		m_input->Initialize();

		// Create the graphics object.  This object will handle rendering all the graphics for this application.
		m_Graphics = std::make_unique<Graphics>();
		if (!m_Graphics)
		{
			return false;
		}

		// Initialize the graphics object.
		result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
		if (!result)
		{
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------------------------------------
	void App::shutdown()
	{
		// Release the graphics object.
		if (m_graphics)
		{
			m_graphics->Shutdown();
			m_graphics.reset(nullptr);
		}

		if (m_input)
		{
			m_input.reset(nullptr);
		}

		// Shutdown the window.
		shutdownWindows();

		return;
	}

	//-----------------------------------------------------------------------------------------------------
	void App::run()
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
				result = frame();
				if (!result)
				{
					done = true;
				}
			}

		}

		return;
	}

	//-----------------------------------------------------------------------------------------------------
	bool App::frame()
	{
		bool result;


		// Check if the user pressed escape and wants to exit the application.
		if (m_input->IsKeyDown(VK_ESCAPE))
		{
			return false;
		}

		// Do the frame processing for the graphics object.
		result = m_graphics->Frame();
		if (!result)
		{
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------------------------------------
	LRESULT CALLBACK App::messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
			// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
			{
				// If a key is pressed send it to the input object so it can record that state.
				m_input->KeyDown((unsigned int)wparam);
				return 0;
			}

			// Check if a key has been released on the keyboard.
			case WM_KEYUP:
			{
				// If a key is released then send it to the input object so it can unset the state for that key.
				m_input->KeyUp((unsigned int)wparam);
				return 0;
			}

			// Any other messages send to the default message handler as our application won't make use of them.
			default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------
	void App::initializeWindows(int& screenWidth, int& screenHeight)
	{
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;

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
		screenWidth = GetAppMetrics(SM_CXSCREEN);
		screenHeight = GetAppMetrics(SM_CYSCREEN);

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
		}
		else
		{
			// If windowed then set it to 800x600 resolution.
			screenWidth = 800;
			screenHeight = 600;

			// Place the window in the middle of the screen.
			posX = (GetAppMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetAppMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}

		// Create the window with the screen settings and get the handle to it.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);

		// Hide the mouse cursor.
		ShowCursor(false);

		return;
	}

	//-----------------------------------------------------------------------------------------------------
	void App::shutdownWindows()
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


		return;
	}

	//-----------------------------------------------------------------------------------------------------
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

			// All other messages pass to the message handler in the App class.
			default:
			{
				return App::instance()->messageHandler(hwnd, umessage, wparam, lparam);
			}
		}
	}

}