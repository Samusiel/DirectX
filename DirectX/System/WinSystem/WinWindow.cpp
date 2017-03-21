#include "stdafx.h"
#include "WinWindow.h"

namespace System
{

	WinWindow::WinWindow() : m_hinst(nullptr), m_hwnd(nullptr)
	{
	}

	WinWindow::~WinWindow()
	{
		m_hinst = nullptr;
		m_hwnd = nullptr;
	}

	Result WinWindow::onInitialize(const Math::Rect &bound)
	{
		static const wchar_t WndClassName[] = L"WinWindow_WndClass";
		static const wchar_t WndTitle[] = L"ChaosEngine";

		// get active process handle

		this->m_hinst = GetModuleHandle(NULL);

		// register window class

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.lpszClassName = WndClassName;
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WinWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hinst;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.hIconSm = nullptr;

		//if (!RegisterClassExW(&wcex))
		//	return ResultError(Failure, "WinCanvas: failed to register window class");

		// create window

		this->m_hwnd = CreateWindowW(WndClassName, WndTitle, WS_OVERLAPPED | WS_POPUP,
			bound.Min.x, bound.Min.y, bound.Max.x, bound.Max.y,
			nullptr, nullptr, this->m_hinst, this);
		//if (!this->m_hwnd)
		//	ResultError(Failure, "WinCanvas: failed to create window");

		ShowWindow(this->m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(this->m_hwnd);

		return Result(Success);// ResultNote(Success, "WinCanvas: initialized");
	}

	Result WinWindow::onSetBound(const Math::Rect& bound)
	{
		if (nullptr == this->m_hwnd)
			assert(false && NotInitialized && "WinCanvas: trying to move uninitialized windows");

		BOOL res = MoveWindow(this->m_hwnd, bound.Min.x, bound.Min.y, bound.Max.x, bound.Max.y, FALSE);

		return TRUE == res ? Result(Success) : Result(Failure);// ResultError(Failure, "WinCanvas: failed to move window"));
	}

	LRESULT CALLBACK WinWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WinWindow *window = nullptr;

		if (WM_NCCREATE == message)
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			window = static_cast<WinWindow*>(lpcs->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}
		else
		{
			window = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		switch (message)
		{
		case WM_SIZE:
		case WM_MOVE:
		{
			RECT wndRect;
			if (window != nullptr &&
				window->m_hwnd != nullptr &&
				GetWindowRect(hWnd, &wndRect))
			{
				window->setBound(Math::Rect(wndRect.left, wndRect.top, wndRect.right, wndRect.bottom));
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			if (window != nullptr)
			{
				// redraw
			}
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

}