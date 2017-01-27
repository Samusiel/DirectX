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
	App::App() : m_renderSystem(nullptr), m_window(nullptr), m_graphics(nullptr), m_input(nullptr)
	{
	}

	//-----------------------------------------------------------------------------------------------------
	App::~App()
	{
		m_app = nullptr;
	}

	//-----------------------------------------------------------------------------------------------------
	Result App::init()
	{
		//-- init subsytems
		return Result(Success);
	}

	//-----------------------------------------------------------------------------------------------------
	void App::input(Input* input)
	{
		m_input.reset(input);
	}

	//-----------------------------------------------------------------------------------------------------
	void App::window(Window* window)
	{
		m_window.reset(window);
	}

}