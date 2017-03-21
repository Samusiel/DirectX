#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base input system
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
namespace System
{

	class Input
	{
	public:

		typedef size_t KeyCode;
		static const KeyCode InvalidCode = ~KeyCode(0);

		enum class KeyState : uint8
		{
			Up,
			Down,
			Released,
		};

		enum class VKey : uint8
		{
			Num0,
			Num1,
			Num2,
			Num3,
			Num4,
			Num5,
			Num6,
			Num7,
			Num8,
			Num9,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			Oem1,		// ';:'
			Oem2,		// '/?'
			Oem3,		// '`~'
			Oem4,		// '[{'
			Oem5,		// '\|'
			Oem6,		// ']}'
			Oem7,		// ''"'
			Oem8,
			OemPlus,	// '+'
			OemComma,	// ','
			OemMinus,	// '-'
			OemPeriod,	// '.'
			Back,
			Tab,
			Return,
			Pause,
			Escape,
			Space,
			Prior,
			Next,
			End,
			Home,
			Left,
			Up,
			Right,
			Down,
			Insert,
			Delete,
			NumLock,
			NumPad0,
			NumPad1,
			NumPad2,
			NumPad3,
			NumPad4,
			NumPad5,
			NumPad6,
			NumPad7,
			NumPad8,
			NumPad9,
			Multiply,
			Add,
			Separator,
			Subtract,
			Decimal,
			Divide,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			Shift,
			Control,
			Alt,
			LButton,
			RButton,
			MButton,
			Count,
			Undefined = 0xff
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Basic input device
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class Device
		{
		public:

			Device(Input &input);

			~Device();

			virtual Result initialize();

			virtual void update();

			inline Input& getInput();

		private:

			Input &input;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Keyboard state class
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class Keyboard : public Device
		{
		public:

			Keyboard(Input &input);

			virtual ~Keyboard();

			virtual Result initialize();

			inline size_t getInputQueueSize();

			inline KeyCode getInputQueueChar(size_t idx);

			inline KeyCode getKeyCode(VKey vkey);

			inline KeyState getKeyState(VKey vkey);

			inline bool isKeyUp(VKey vkey);

			inline bool isKeyDown(VKey vkey);

			inline bool isKeyReleased(VKey vkey);

		protected:

			static const size_t InputQueueMaxSize = 256;

			KeyState	m_vkeyState[0xff];
			KeyCode		m_vkeyToCode[0xff];
			VKey		m_codeToVKey[0xff];
			KeyCode		m_inputQueue[InputQueueMaxSize];
			size_t		m_inputQueueSize;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Mouse state class
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class Mouse : public Device
		{
		public:

			Mouse(Input &input);

			virtual ~Mouse();

			virtual Result initialize();

			inline const Math::Vector2& getPos() const;

			inline KeyState getLBState() const;

			inline KeyState getMBState() const;

			inline KeyState getRBState() const;

			inline int getWheelDelta() const;

		protected:

			Math::Vector2	m_pos;
			KeyState		m_lbState;
			KeyState		m_mbState;
			KeyState		m_rbState;
			int				m_wheelDelta;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Gamepad state class
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class Gamepad : public Device
		{
		public:

			static const size_t MaxConnected = 4;

			Gamepad(Input &input);

			virtual ~Gamepad();

			virtual Result initialize();

			// todo: not implemented yet
		};


		Input();

		virtual ~Input();

		virtual Result initialize();

		virtual void update();

		inline Keyboard* getKeyboard() const;

		inline Mouse* getMouse() const;

		Gamepad* getGamepad(size_t idx) const;

	protected:

		std::unique_ptr<Keyboard> m_keyboard;
		std::unique_ptr<Mouse> m_mouse;
		std::unique_ptr<Gamepad> m_gamepad[Gamepad::MaxConnected];
	};

}

#include "Input.ipp"