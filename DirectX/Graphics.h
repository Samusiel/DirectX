#pragma once
//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include "d3ddevice.h"

class Camera;
class Model;
class ColorShader;


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	std::unique_ptr<D3DDevice> m_direct3D;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Model> m_model;
	std::unique_ptr<ColorShader> m_colorShader;
};

