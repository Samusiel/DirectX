#include "stdafx.h"
#include "Graphics.h"
#include "Camera.h"
#include "ColorShader.h"
#include "Model.h"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}


Graphics::Graphics(const Graphics& other)
{
}


bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_direct3D = std::make_unique<D3DDevice>();
	if (m_direct3D == nullptr)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_camera = std::make_unique<Camera>(1, 2, 25, 0.5);
	if (m_camera == nullptr)
	{
		return false;
	}

	// Create the model object.
	m_model = std::make_unique<Model>();
	if (m_model == nullptr)
	{
		return false;
	}

	// Initialize the model object.
	result = m_model->Initialize(m_direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_colorShader = std::make_unique<ColorShader>();
	if (m_colorShader == nullptr)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_colorShader->Initialize(m_direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown()
{
	// Release the color shader object.
	if (m_colorShader != nullptr)
	{
		m_colorShader->Shutdown();
		m_colorShader.reset(nullptr);
	}

	// Release the model object.
	if (m_model)
	{
		m_model->Shutdown();
		m_colorShader.reset(nullptr);
	}

	// Release the camera object.
	if (m_camera)
	{
		m_camera.reset(nullptr);
	}

	// Release the Direct3D object.
	if (m_direct3D != nullptr)
	{
		m_direct3D->Shutdown();
		m_direct3D.reset(nullptr);
	}
}


bool Graphics::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool Graphics::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_direct3D->BeginScene(0.0f, 1.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_direct3D->GetWorldMatrix(worldMatrix);
	//m_camera->GetViewMatrix(viewMatrix);
	m_direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_model->Render(m_direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_colorShader->Render(m_direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_direct3D->EndScene();

	return true;
}
