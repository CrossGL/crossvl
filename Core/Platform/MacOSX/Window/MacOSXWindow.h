#pragma once
#include <Core/Common.h>

namespace CGL::Core::Platform::MacOSX
{
	class Window {
	public:
		explicit Window(std::string_view title, u16 width, u16 height);
		~Window();

		inline bool IsOpen() const { return is_open; }

		void Show();
		void Close();

		void Update();
		void ProcessMessages();

	private:
		bool is_open;
	};
}
