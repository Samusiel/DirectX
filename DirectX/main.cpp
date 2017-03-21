#include "stdafx.h"
#include "System/App.h"

#include "System\WinSystem\WinInput.h"
#include "System\WinSystem\WinWindow.h"

using namespace System;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// create realm
	App::instance()->init();

	// create system canvas
	size_t screenWidth = size_t(GetSystemMetrics(SM_CXSCREEN));
	size_t screenHeight = size_t(GetSystemMetrics(SM_CYSCREEN));
	WinWindow* window = new WinWindow();
	window->initialize(Math::Rect(0, 0, screenWidth, screenHeight));
	App::instance()->window(window);

	// create input system
	WinInput* input = new WinInput();
	input->initialize();
	App::instance()->input(input);

	// create Graphics system
	std::unique_ptr<GraphicsSystemD3D11> Graphics(new GraphicsSystemD3D11(realm));
	Result res = Graphics->Initialize(realm.GetCanvas());
	realm.SetGraphicsSystem(std::move(Graphics));

	// create swap chain
	std::unique_ptr<GraphicsSwapChain> GraphicsSwapChain;
	if (Succeeded(realm.GetGraphicsSystem()->CreateSwapChain(GraphicsSwapChain)))
	{
		res = GraphicsSwapChain->Initialize(screenWidth, screenHeight);
	}

	// create Graphics context
	std::unique_ptr<GraphicsContext> GraphicsContext;
	if (Succeeded(realm.GetGraphicsSystem()->CreateContext(GraphicsContext)))
	{
		res = GraphicsContext->Initialize();
	}

	// initialize ImguiRender
	ImguiRender *imguiRender = ur_null;
	if (realm.AddComponent<ImguiRender>(realm))
	{
		imguiRender = realm.GetComponent<ImguiRender>();
		res = imguiRender->Init();
	}

	// initialize GenericRender
	GenericRender *genericRender = ur_null;
	if (realm.AddComponent<GenericRender>(realm))
	{
		genericRender = realm.GetComponent<GenericRender>();
		genericRender->Init();
	}

	// demo camera
	Camera camera(realm);
	CameraControl cameraControl(realm, &camera, CameraControl::Mode::AroundPoint);
	camera.SetPosition(ur_float3(0.0f, 0.0f, -10.0f));
	cameraControl.SetTargetPoint(ur_float3(0.0f));

	// demo isosurface
	std::unique_ptr<Isosurface> isosurface(new Isosurface(realm));
	{
		BoundingBox volumeBound(ur_float3(-4.0f, -4.0f, -4.0f), ur_float3(4.0f, 4.0f, 4.0f));
#if 1
		Isosurface::ProceduralGenerator::SphericalDistanceFieldParams generateParams;
		generateParams.bound = volumeBound;
		generateParams.center = volumeBound.Center();
		generateParams.radius = volumeBound.SizeMin() * 0.5f;
		std::unique_ptr<Isosurface::ProceduralGenerator> dataVolume(new Isosurface::ProceduralGenerator(*isosurface.get(),
			Isosurface::ProceduralGenerator::Algorithm::SphericalDistanceField, generateParams));
#else
		Isosurface::ProceduralGenerator::SimplexNoiseParams generateParams;
		generateParams.bound = volumeBound;
		std::unique_ptr<Isosurface::ProceduralGenerator> dataVolume(new Isosurface::ProceduralGenerator(*isosurface.get(),
			Isosurface::ProceduralGenerator::Algorithm::SimplexNoise, generateParams));
#endif

		Isosurface::HybridCubes::Desc desc;
		desc.CellSize = 0.05f;
		desc.LatticeResolution = 16;
		desc.DetailLevelDistance = desc.CellSize * desc.LatticeResolution.x;
		std::unique_ptr<Isosurface::HybridCubes> presentation(new Isosurface::HybridCubes(*isosurface.get(), desc));

		isosurface->Init(std::move(dataVolume), std::move(presentation));
	}

	// Main message loop:
	realm.GetLog().WriteLine("Entering main message loop");
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		
	}
	realm.GetLog().WriteLine("Left main message loop");

	return 0;//(int)msg.wParam;
}