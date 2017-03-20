#pragma once

#include <windows.h>

#include "System/Window.hpp"

namespace System
{

	class WinWindow : public Window
	{
	public:
		WinWindow();

		virtual ~WinWindow();

		inline HINSTANCE getHinstance() const
		{
			return m_hinst;
		}

		inline HWND getHwnd() const
		{
			return m_hwnd;
		}

	private:

		virtual Result onInitialize(const Math::Rect &bound);

		virtual Result onSetBound(const Math::Rect &bound);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE m_hinst;
		HWND m_hwnd;
	};

}
