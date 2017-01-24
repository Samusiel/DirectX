#pragma once

namespace System
{

	class Window
	{
	public:
		Window();
		virtual ~Window();
		
		Result initialize(const Math::Rect& bound);

		Result setBound(const Math::Rect& bound);

		inline void setPos(int x, int y);

		inline void setSize(int width, int height);

		inline const Math::Rect& getBound() const;

	protected:

		virtual Result onInitialize(const Math::Rect &bound);

		virtual Result onSetBound(const Math::Rect &bound);

	private:

		Math::Rect m_bound;
	};

}

