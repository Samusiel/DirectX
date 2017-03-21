#include "stdafx.h"
#include "App.h"
#include "Input.h"
#include "Graphics/GraphicsSystem.hpp"
#include "Window.hpp"

namespace System
{
	//-----------------------------------------------------------------------------------------------------
	App& App::instance()
	{
		static App s_instance;
		return s_instance;
	}

	//-----------------------------------------------------------------------------------------------------
	App::App() : m_graphicsSystem(nullptr), m_window(nullptr), m_input(nullptr)
	{
	}

	//-----------------------------------------------------------------------------------------------------
	App::~App()
	{
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