#pragma once
#include <Core/Common.h>
#include <Core/Platform/Win32/Win32Utils.h>

namespace CGL::Core::Platform::Win32
{
	template <class DERIVED_TYPE>
	class BaseWindow
	{
	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DERIVED_TYPE* pThis = NULL;

			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->m_hWnd = hWnd;
			}
			else
			{
				pThis = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}
			if (pThis)
			{
				return pThis->HandleMessage(uMsg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}

		BaseWindow() : m_hWnd(NULL) { }

		BOOL Create(
			std::string_view lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x           = CW_USEDEFAULT,
			int y           = CW_USEDEFAULT,
			int nWidth      = CW_USEDEFAULT,
			int nHeight     = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu     = 0
		)
		{
			WNDCLASSEX wc{};
			ZeroMemory(&wc, sizeof(WNDCLASSEX));

			wc.cbSize        = sizeof(WNDCLASSEX);
			wc.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
			wc.hIcon         = NULL;
			wc.hCursor       = NULL;
			wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName  = NULL;
			wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
			wc.hInstance     = GetModuleHandle(NULL);

			std::string className(ClassName());
			wc.lpszClassName = className.c_str();

			RegisterClassEx(&wc);

			std::string title(lpWindowName);
			m_hWnd = CreateWindowEx(
				dwExStyle, className.c_str(), title.c_str(), dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
			);

			return (m_hWnd ? TRUE : FALSE);
		}

		HWND Handle() const { return m_hWnd; }
		operator HWND() const noexcept { return m_hWnd; }

	protected:

		virtual std::string_view ClassName() const = 0;
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

		HWND m_hWnd;
	};

	class Window : public BaseWindow<Window>
	{
	public:
		explicit Window(std::string_view title, u32 width, u32 height);
		~Window();
		
		inline bool IsOpen() const { return m_isOpen; }

		void Show();
		void Close();
		void ProcessMessages();

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	protected:
		virtual void OnResize([[maybe_unused]] u32 width, [[maybe_unused]] u32 height) {}

	private:
		std::string_view ClassName() const override final;

	private:
		bool m_isOpen;
	};
}
