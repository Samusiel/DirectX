#pragma once

namespace System
{

	inline Input& Input::Device::getInput()
	{
		return this->input;
	}

	inline Input::Keyboard* Input::getKeyboard() const
	{
		return this->m_keyboard.get();
	}

	inline Input::Mouse* Input::getMouse() const
	{
		return this->m_mouse.get();
	}

	inline Input::Gamepad* Input::getGamepad(size_t idx) const
	{
		return this->m_gamepad[min(idx, size_t(Input::Gamepad::MaxConnected - 1))].get();
	}

	inline size_t Input::Keyboard::getInputQueueSize()
	{
		return this->m_inputQueueSize;
	}

	inline Input::KeyCode Input::Keyboard::getInputQueueChar(size_t idx)
	{
		return (idx < this->m_inputQueueSize ? this->m_inputQueue[idx] : Input::InvalidCode);
	}

	inline Input::KeyCode Input::Keyboard::getKeyCode(VKey vkey)
	{
		return ((size_t)vkey < (size_t)VKey::Count ? this->m_vkeyToCode[(size_t)vkey] : Input::InvalidCode);
	}

	inline Input::KeyState Input::Keyboard::getKeyState(VKey vkey)
	{
		return this->m_vkeyState[(size_t)vkey];
	}

	inline bool Input::Keyboard::isKeyUp(VKey vkey)
	{
		return (KeyState::Up == this->getKeyState(vkey));
	}

	inline bool Input::Keyboard::isKeyDown(VKey vkey)
	{
		return (KeyState::Down == this->getKeyState(vkey));
	}

	inline bool Input::Keyboard::isKeyReleased(VKey vkey)
	{
		return (KeyState::Released == this->getKeyState(vkey));
	}

	inline const Math::Vector2& Input::Mouse::getPos() const
	{
		return this->m_pos;
	}

	inline Input::KeyState Input::Mouse::getLBState() const
	{
		return this->m_lbState;
	}

	inline Input::KeyState Input::Mouse::getMBState() const
	{
		return this->m_mbState;
	}

	inline Input::KeyState Input::Mouse::getRBState() const
	{
		return this->m_rbState;
	}

	inline int Input::Mouse::getWheelDelta() const
	{
		return this->m_wheelDelta;
	}

}
