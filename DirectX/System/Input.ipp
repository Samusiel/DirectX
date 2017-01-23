#pragma once

namespace System
{

	inline Input& Input::Device::GetInput()
	{
		return this->input;
	}

	inline Input::Keyboard* Input::GetKeyboard() const
	{
		return this->keyboard.get();
	}

	inline Input::Mouse* Input::GetMouse() const
	{
		return this->mouse.get();
	}

	inline Input::Gamepad* Input::GetGamepad(size_t idx) const
	{
		return this->gamepad[min(idx, size_t(Input::Gamepad::MaxConnected - 1))].get();
	}

	inline size_t Input::Keyboard::GetInputQueueSize()
	{
		return this->m_inputQueueSize;
	}

	inline Input::KeyCode Input::Keyboard::GetInputQueueChar(size_t idx)
	{
		return (idx < this->m_inputQueueSize ? this->m_inputQueue[idx] : Input::InvalidCode);
	}

	inline Input::KeyCode Input::Keyboard::GetKeyCode(VKey vkey)
	{
		return ((size_t)vkey < (size_t)VKey::Count ? this->m_vkeyToCode[(size_t)vkey] : Input::InvalidCode);
	}

	inline Input::KeyState Input::Keyboard::GetKeyState(VKey vkey)
	{
		return this->m_vkeyState[(size_t)vkey];
	}

	inline bool Input::Keyboard::IsKeyUp(VKey vkey)
	{
		return (KeyState::Up == this->GetKeyState(vkey));
	}

	inline bool Input::Keyboard::IsKeyDown(VKey vkey)
	{
		return (KeyState::Down == this->GetKeyState(vkey));
	}

	inline bool Input::Keyboard::IsKeyReleased(VKey vkey)
	{
		return (KeyState::Released == this->GetKeyState(vkey));
	}

	inline const Math::Vector2& Input::Mouse::GetPos() const
	{
		return this->m_pos;
	}

	inline Input::KeyState Input::Mouse::GetLBState() const
	{
		return this->m_lbState;
	}

	inline Input::KeyState Input::Mouse::GetMBState() const
	{
		return this->m_mbState;
	}

	inline Input::KeyState Input::Mouse::GetRBState() const
	{
		return this->m_rbState;
	}

	inline int Input::Mouse::GetWheelDelta() const
	{
		return this->m_wheelDelta;
	}

}
