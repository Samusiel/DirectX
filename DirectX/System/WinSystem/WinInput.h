#pragma once

#include "../Input.h"

namespace System
{

	class WinInput : public Input
	{
	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Keyboard implementation
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class WinKeyboard : public Keyboard
		{
		public:

			WinKeyboard(WinInput &winInput);

			~WinKeyboard();

			virtual Result Initialize();

			virtual void Update();
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Mouse implementation
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		class WinMouse : public Mouse
		{
		public:

			WinMouse(WinInput &winInput);

			~WinMouse();

			virtual Result Initialize();

			virtual void Update();
		};


		WinInput();

		virtual ~WinInput();

		virtual Result Initialize();

		virtual void Update();

		void ProcessMsg(const MSG &msg);

	private:

		friend class WinKeyboard;
		friend class WinMouse;

		std::vector<MSG> m_msgQueue;
	};

}

