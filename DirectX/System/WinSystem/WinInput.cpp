#include "stdafx.h"
#include "WinInput.h"

namespace System
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WinInput
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	WinInput::WinInput()
	{
		this->m_msgQueue.reserve(128);
	}

	WinInput::~WinInput()
	{

	}

	Result WinInput::initialize()
	{
		// initialize keyboard

		std::unique_ptr<WinKeyboard> winKeyboard(new WinKeyboard(*this));
		if (Succeeded(winKeyboard->initialize()))
		{
			this->m_keyboard = std::move(winKeyboard);
		}

		// initialize mouse
		std::unique_ptr<WinMouse> winMouse(new WinMouse(*this));
		if (Succeeded(winMouse->initialize()))
		{
			this->m_mouse = std::move(winMouse);
		}

		return Result(this->m_keyboard != nullptr && this->m_mouse != nullptr ? Success : Failure);
	}

	void WinInput::update()
	{
		Input::update();

		// queue can now be cleared after all devices have processed the accumulated messages
		this->m_msgQueue.clear();
	}

	void WinInput::ProcessMsg(const MSG &msg)
	{
		this->m_msgQueue.push_back(msg);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WinKeyboard
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	WinInput::WinKeyboard::WinKeyboard(WinInput &winInput) :
		Keyboard(winInput)
	{
		// virtual keys mapping

		this->m_vkeyToCode[(size_t)VKey::Num0] = 0x30;
		this->m_vkeyToCode[(size_t)VKey::Num1] = 0x31;
		this->m_vkeyToCode[(size_t)VKey::Num2] = 0x32;
		this->m_vkeyToCode[(size_t)VKey::Num3] = 0x33;
		this->m_vkeyToCode[(size_t)VKey::Num4] = 0x34;
		this->m_vkeyToCode[(size_t)VKey::Num5] = 0x35;
		this->m_vkeyToCode[(size_t)VKey::Num6] = 0x36;
		this->m_vkeyToCode[(size_t)VKey::Num7] = 0x37;
		this->m_vkeyToCode[(size_t)VKey::Num8] = 0x38;
		this->m_vkeyToCode[(size_t)VKey::Num9] = 0x39;
		this->m_vkeyToCode[(size_t)VKey::A] = 0x41;
		this->m_vkeyToCode[(size_t)VKey::B] = 0x42;
		this->m_vkeyToCode[(size_t)VKey::C] = 0x43;
		this->m_vkeyToCode[(size_t)VKey::D] = 0x44;
		this->m_vkeyToCode[(size_t)VKey::E] = 0x45;
		this->m_vkeyToCode[(size_t)VKey::F] = 0x46;
		this->m_vkeyToCode[(size_t)VKey::G] = 0x47;
		this->m_vkeyToCode[(size_t)VKey::H] = 0x48;
		this->m_vkeyToCode[(size_t)VKey::I] = 0x49;
		this->m_vkeyToCode[(size_t)VKey::J] = 0x4A;
		this->m_vkeyToCode[(size_t)VKey::K] = 0x4B;
		this->m_vkeyToCode[(size_t)VKey::L] = 0x4C;
		this->m_vkeyToCode[(size_t)VKey::M] = 0x4D;
		this->m_vkeyToCode[(size_t)VKey::N] = 0x4E;
		this->m_vkeyToCode[(size_t)VKey::O] = 0x4F;
		this->m_vkeyToCode[(size_t)VKey::P] = 0x50;
		this->m_vkeyToCode[(size_t)VKey::Q] = 0x51;
		this->m_vkeyToCode[(size_t)VKey::R] = 0x52;
		this->m_vkeyToCode[(size_t)VKey::S] = 0x53;
		this->m_vkeyToCode[(size_t)VKey::T] = 0x54;
		this->m_vkeyToCode[(size_t)VKey::U] = 0x55;
		this->m_vkeyToCode[(size_t)VKey::V] = 0x56;
		this->m_vkeyToCode[(size_t)VKey::W] = 0x57;
		this->m_vkeyToCode[(size_t)VKey::X] = 0x58;
		this->m_vkeyToCode[(size_t)VKey::Y] = 0x59;
		this->m_vkeyToCode[(size_t)VKey::Z] = 0x5A;
		this->m_vkeyToCode[(size_t)VKey::Oem1] = VK_OEM_1;
		this->m_vkeyToCode[(size_t)VKey::Oem2] = VK_OEM_2;
		this->m_vkeyToCode[(size_t)VKey::Oem3] = VK_OEM_3;
		this->m_vkeyToCode[(size_t)VKey::Oem4] = VK_OEM_4;
		this->m_vkeyToCode[(size_t)VKey::Oem5] = VK_OEM_5;
		this->m_vkeyToCode[(size_t)VKey::Oem6] = VK_OEM_6;
		this->m_vkeyToCode[(size_t)VKey::Oem7] = VK_OEM_7;
		this->m_vkeyToCode[(size_t)VKey::Oem8] = VK_OEM_8;
		this->m_vkeyToCode[(size_t)VKey::OemPlus] = VK_OEM_PLUS;
		this->m_vkeyToCode[(size_t)VKey::OemComma] = VK_OEM_COMMA;
		this->m_vkeyToCode[(size_t)VKey::OemMinus] = VK_OEM_MINUS;
		this->m_vkeyToCode[(size_t)VKey::OemPeriod] = VK_OEM_PERIOD;
		this->m_vkeyToCode[(size_t)VKey::Back] = VK_BACK;
		this->m_vkeyToCode[(size_t)VKey::Tab] = VK_TAB;
		this->m_vkeyToCode[(size_t)VKey::Return] = VK_RETURN;
		this->m_vkeyToCode[(size_t)VKey::Pause] = VK_PAUSE;
		this->m_vkeyToCode[(size_t)VKey::Escape] = VK_ESCAPE;
		this->m_vkeyToCode[(size_t)VKey::Space] = VK_SPACE;
		this->m_vkeyToCode[(size_t)VKey::Prior] = VK_PRIOR;
		this->m_vkeyToCode[(size_t)VKey::Next] = VK_NEXT;
		this->m_vkeyToCode[(size_t)VKey::End] = VK_END;
		this->m_vkeyToCode[(size_t)VKey::Home] = VK_HOME;
		this->m_vkeyToCode[(size_t)VKey::Left] = VK_LEFT;
		this->m_vkeyToCode[(size_t)VKey::Up] = VK_UP;
		this->m_vkeyToCode[(size_t)VKey::Right] = VK_RIGHT;
		this->m_vkeyToCode[(size_t)VKey::Down] = VK_DOWN;
		this->m_vkeyToCode[(size_t)VKey::Insert] = VK_INSERT;
		this->m_vkeyToCode[(size_t)VKey::Delete] = VK_DELETE;
		this->m_vkeyToCode[(size_t)VKey::NumLock] = VK_NUMLOCK;
		this->m_vkeyToCode[(size_t)VKey::NumPad0] = VK_NUMPAD0;
		this->m_vkeyToCode[(size_t)VKey::NumPad1] = VK_NUMPAD1;
		this->m_vkeyToCode[(size_t)VKey::NumPad2] = VK_NUMPAD2;
		this->m_vkeyToCode[(size_t)VKey::NumPad3] = VK_NUMPAD3;
		this->m_vkeyToCode[(size_t)VKey::NumPad4] = VK_NUMPAD4;
		this->m_vkeyToCode[(size_t)VKey::NumPad5] = VK_NUMPAD5;
		this->m_vkeyToCode[(size_t)VKey::NumPad6] = VK_NUMPAD6;
		this->m_vkeyToCode[(size_t)VKey::NumPad7] = VK_NUMPAD7;
		this->m_vkeyToCode[(size_t)VKey::NumPad8] = VK_NUMPAD8;
		this->m_vkeyToCode[(size_t)VKey::NumPad9] = VK_NUMPAD9;
		this->m_vkeyToCode[(size_t)VKey::Multiply] = VK_MULTIPLY;
		this->m_vkeyToCode[(size_t)VKey::Add] = VK_ADD;
		this->m_vkeyToCode[(size_t)VKey::Separator] = VK_SEPARATOR;
		this->m_vkeyToCode[(size_t)VKey::Subtract] = VK_SUBTRACT;
		this->m_vkeyToCode[(size_t)VKey::Decimal] = VK_DECIMAL;
		this->m_vkeyToCode[(size_t)VKey::Divide] = VK_DIVIDE;
		this->m_vkeyToCode[(size_t)VKey::F1] = VK_F1;
		this->m_vkeyToCode[(size_t)VKey::F2] = VK_F2;
		this->m_vkeyToCode[(size_t)VKey::F3] = VK_F3;
		this->m_vkeyToCode[(size_t)VKey::F4] = VK_F4;
		this->m_vkeyToCode[(size_t)VKey::F5] = VK_F5;
		this->m_vkeyToCode[(size_t)VKey::F6] = VK_F6;
		this->m_vkeyToCode[(size_t)VKey::F7] = VK_F7;
		this->m_vkeyToCode[(size_t)VKey::F8] = VK_F8;
		this->m_vkeyToCode[(size_t)VKey::F9] = VK_F9;
		this->m_vkeyToCode[(size_t)VKey::F10] = VK_F10;
		this->m_vkeyToCode[(size_t)VKey::F11] = VK_F11;
		this->m_vkeyToCode[(size_t)VKey::F12] = VK_F12;
		this->m_vkeyToCode[(size_t)VKey::Shift] = VK_SHIFT;
		this->m_vkeyToCode[(size_t)VKey::Control] = VK_CONTROL;
		this->m_vkeyToCode[(size_t)VKey::Alt] = VK_MENU;

		for (size_t i = 0; i < (size_t)VKey::Count; ++i)
		{
			this->m_codeToVKey[this->m_vkeyToCode[i]] = (VKey)i;
		}
	}

	WinInput::WinKeyboard::~WinKeyboard()
	{

	}

	Result WinInput::WinKeyboard::initialize()
	{
		return Result(Success);
	}

	void WinInput::WinKeyboard::update()
	{
		this->m_inputQueueSize = 0;
		for (size_t i = 0; i < (size_t)VKey::Count; ++i)
		{
			// reset released keys to "Up" state 
			if (this->m_vkeyState[i] == KeyState::Released)
				this->m_vkeyState[i] = KeyState::Up;
		}

		WinInput &winInput = static_cast<WinInput&>(this->getInput());
		for (const MSG &msg : winInput.m_msgQueue)
		{
			switch (msg.message)
			{
			case WM_KEYDOWN:
			{
				size_t vkey = (size_t)this->m_codeToVKey[msg.wParam];
				if (vkey < (size_t)VKey::Count)
				{
					this->m_vkeyState[vkey] = KeyState::Down;
				}
			}
			break;
			case WM_KEYUP:
			{
				size_t vkey = (size_t)this->m_codeToVKey[msg.wParam];
				if (vkey < (size_t)VKey::Count)
				{
					this->m_vkeyState[vkey] = KeyState::Released;
				}
			}
			break;
			case WM_CHAR:
			{
				if (this->m_inputQueueSize < InputQueueMaxSize)
				{
					this->m_inputQueue[this->m_inputQueueSize++] = (KeyCode)msg.wParam;
				}
			}
			break;
			}
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WinMouse
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	WinInput::WinMouse::WinMouse(WinInput &winInput) :
		Mouse(winInput)
	{

	}

	WinInput::WinMouse::~WinMouse()
	{

	}

	Result WinInput::WinMouse::initialize()
	{
		return Result(Success);
	}

	void WinInput::WinMouse::update()
	{
		if (this->m_lbState == KeyState::Released) this->m_lbState = KeyState::Up;
		if (this->m_rbState == KeyState::Released) this->m_rbState = KeyState::Up;
		if (this->m_mbState == KeyState::Released) this->m_mbState = KeyState::Up;
		this->m_wheelDelta = 0;

		WinInput &winInput = static_cast<WinInput&>(this->getInput());
		for (const MSG &msg : winInput.m_msgQueue)
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				this->m_lbState = KeyState::Down;
				break;
			case WM_LBUTTONUP:
				this->m_lbState = KeyState::Released;
				break;
			case WM_RBUTTONDOWN:
				this->m_rbState = KeyState::Down;
				break;
			case WM_RBUTTONUP:
				this->m_rbState = KeyState::Released;
				break;
			case WM_MBUTTONDOWN:
				this->m_mbState = KeyState::Down;
				break;
			case WM_MBUTTONUP:
				this->m_mbState = KeyState::Released;
				break;
			case WM_MOUSEWHEEL:
				this->m_wheelDelta += (int)GET_WHEEL_DELTA_WPARAM(msg.wParam);
				break;
			case WM_MOUSEMOVE:
				this->m_pos.x = (float)LOWORD(msg.lParam);
				this->m_pos.y = (float)HIWORD(msg.lParam);
				break;
			}
		}
	}

}
