#pragma once

//-- Forward declarations
namespace Graphics
{
	class GraphicsSystem;
}

namespace System
{
	class Input;
	class Window;
}

namespace System
{
	class App;
	typedef std::unique_ptr<App> AppPtr;

	class App
	{
	public:
		//-- private constructor for Singleton Pattern
		App();

		//-- destructor
		~App();

		//-- another public methods
		Result init();

		//-- getters
		Input* input() { return m_input.get(); }
		const Input* input() const { return m_input.get(); }

		Graphics::GraphicsSystem* GraphicsSystem() { return m_graphicsSystem.get(); }
		const Graphics::GraphicsSystem* GraphicsSystem() const { return m_graphicsSystem.get(); }

		Window* window() { return m_window.get(); }
		const Window* window() const { return m_window.get(); }

		//-- setters
		void input(Input* input);
		void window(Window* window);

		static App& instance();
	private:
		
		
	private:
		std::unique_ptr<Input> m_input;
		std::unique_ptr<Graphics::GraphicsSystem> m_graphicsSystem;
		std::unique_ptr<Window> m_window;
	};

}