#include "stdafx.h"
#include "Input.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Device
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
namespace System
{

	Input::Device::Device(Input &input) :
		input(input)
	{

	}

	Input::Device::~Device()
	{

	}

	Result Input::Device::initialize()
	{
		return Result(NotImplemented);
	}

	void Input::Device::update()
	{

	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Keyboard
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	Input::Keyboard::Keyboard(Input &input) :
		Input::Device(input)
	{
		memset(&this->m_vkeyState,  0x00, sizeof(this->m_vkeyState));
		memset(&this->m_vkeyToCode, 0xff, sizeof(this->m_vkeyToCode));
		memset(&this->m_codeToVKey, 0xff, sizeof(this->m_codeToVKey));
		memset(&this->m_inputQueue, 0xff, sizeof(this->m_inputQueue));
		this->m_inputQueueSize = 0;
	}

	Input::Keyboard::~Keyboard()
	{

	}

	Result Input::Keyboard::initialize()
	{
		return Result(NotImplemented);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mouse
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Input::Mouse::Mouse(Input &input) :
		Input::Device(input)
	{
		this->m_lbState = KeyState::Up;
		this->m_mbState = KeyState::Up;
		this->m_rbState = KeyState::Up;
		this->m_wheelDelta = 0;
	}

	Input::Mouse::~Mouse()
	{

	}

	Result Input::Mouse::initialize()
	{
		return Result(NotImplemented);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Gamepad
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Input::Gamepad::Gamepad(Input &input) :
		Input::Device(input)
	{

	}

	Input::Gamepad::~Gamepad()
	{

	}

	Result Input::Gamepad::initialize()
	{
		return Result(NotImplemented);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Input
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Input::Input()
	{

	}

	Input::~Input()
	{

	}

	Result Input::initialize()
	{
		return Result(NotImplemented);
	}

	void Input::update()
	{
		// update all devices

		if (this->m_keyboard.get() != nullptr)
		{
			this->m_keyboard->update();
		}

		if (this->m_mouse.get() != nullptr)
		{
			this->m_mouse->update();
		}

		for (size_t idx = 0; idx < Gamepad::MaxConnected; ++idx)
		{
			if (this->m_gamepad[idx].get() != nullptr)
			{
				this->m_gamepad[idx]->update();
			}
		}
	}

}
