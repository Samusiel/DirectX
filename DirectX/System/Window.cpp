#include "stdafx.h"
#include "Window.hpp"


namespace System
{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	Result Window::initialize(const Math::Rect& bound)
	{
		if (bound.IsInsideOut())
			return ResultError(InvalidArgs, "Canvas: failed to initialize because of invalid bounding rect");

		m_bound = bound;
		return Success;
	}

	Result Window::setBound(const Math::Rect& bound)
	{

	}

	void Window::setPos(int x, int y)
	{

	}

	void Window::setSize(int width, int height)
	{

	}

	const Math::Rect& Window::getBound() const
	{

	}

	Result Window::OnInitialize(const Math::Rect &bound)
	{

	}

	Result Window::OnSetBound(const Math::Rect &bound)
	{

	}

}