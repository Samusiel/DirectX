#pragma once

namespace System
{

	class Graphics;
	class Input;

	class App;
	typedef std::unique_ptr<App> AppPtr;

	class App
	{
	public:
		~App();

		bool init();
		void shutdown();
		void run();

		LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

		static AppPtr& instance();

	private:
		//--
		App() {}

		bool frame();
		void initializeWindows(int&, int&);
		void shutdownWindows();

	private:
		LPCWSTR m_applicationName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;

		static AppPtr m_app;

		std::unique_ptr<Input> m_input;
		std::unique_ptr<Graphics> m_graphics;
	};

}

