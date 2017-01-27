#pragma once

namespace System
{

	class Graphics;
	class Input;
	class RenderSystem;
	class Window;

	class App;
	typedef std::unique_ptr<App> AppPtr;

	class App
	{
	public:
		//-- Singleton
		static AppPtr& instance();

		//-- destructor
		~App();

		//-- another public methods
		Result init();

		//-- getters
		std::unique_ptr<Input>& input() { return m_input; }
		const std::unique_ptr<Input>& input() const { return m_input; }

		std::unique_ptr<RenderSystem>& renderSystem() { return m_renderSystem; }
		const std::unique_ptr<RenderSystem>& renderSystem() const { return m_renderSystem; }

		std::unique_ptr<Window>& window() { return m_window; }
		const std::unique_ptr<Window>& window() const { return m_window; }

		//-- setters
		void input(Input* input);
		void window(Window* window);


	private:
		//-- private constructor for Singleton Pattern
		App();
		
	private:
		static AppPtr m_app;

		std::unique_ptr<Input> m_input;
		std::unique_ptr<Graphics> m_graphics;
		std::unique_ptr<RenderSystem> m_renderSystem;
		std::unique_ptr<Window> m_window;
	};

}