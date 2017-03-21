#include "stdafx.h"
#include "System/App.h"

#include "System\WinSystem\WinInput.h"
#include "System\WinSystem\WinWindow.h"
#include "Graphics\GraphicsD3D11\GraphicsSystemD3D11.hpp"

using namespace Graphics;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//-- Init application
	App::instance().init();

	// create system canvas
	int screenWidth = int(GetSystemMetrics(SM_CXSCREEN));
	int screenHeight = int(GetSystemMetrics(SM_CYSCREEN));
	WinWindow* window = new WinWindow();
	window->initialize(Math::Rect(0, 0, screenWidth, screenHeight));
	App::instance().window(window);

	// create input system
	WinInput* input = new WinInput();
	input->initialize();
	App::instance().input(input);

	// create Graphics system
	std::unique_ptr<GraphicsSystemD3D11> Graphics(new GraphicsSystemD3D11());
	Result res = Graphics->Initialize(window);

	// create swap chain
	std::unique_ptr<GraphicsSwapChain> GraphicsSwapChain;
	if (Succeeded(App::instance().GraphicsSystem()->CreateSwapChain(GraphicsSwapChain)))
	{
		res = GraphicsSwapChain->Initialize(screenWidth, screenHeight);
	}

	// create Graphics context
	std::unique_ptr<GraphicsContext> GraphicsContext;
	if (Succeeded(App::instance().GraphicsSystem()->CreateContext(GraphicsContext)))
	{
		res = GraphicsContext->Initialize();
	}

	// initialize ImguiRender
	//ImguiRender *imguiRender = nullptr;
	//if (realm.AddComponent<ImguiRender>(realm))
	//{
	//	imguiRender = realm.GetComponent<ImguiRender>();
	//	res = imguiRender->Init();
	//}
	// demo camera
	//Camera camera(realm);
	//CameraControl cameraControl(realm, &camera, CameraControl::Mode::AroundPoint);
	//camera.SetPosition(ur_float3(0.0f, 0.0f, -10.0f));
	//cameraControl.SetTargetPoint(ur_float3(0.0f));
	//
	//MSG msg;
	//ZeroMemory(&msg, sizeof(msg));
	//while (msg.message != WM_QUIT)
	//{
	//	
	//}

	return 0;//(int)msg.wParam;
}