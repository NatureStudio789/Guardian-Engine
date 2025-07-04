#include "GWindow.h"

namespace GE
{
	GWindow::GWindow()
	{
		this->WindowHandle = null;
		this->WindowAttribute = Attribute();
		this->IsWindowRunning = false;
		GUARDIAN_CLEAR_MEMORY(this->WindowMessage);
	}

	GWindow::GWindow(const GWindow& other)
	{
		this->WindowHandle = other.WindowHandle;
		this->WindowAttribute = other.WindowAttribute;
		this->WindowMessage = other.WindowMessage;
		this->IsWindowRunning = other.IsWindowRunning;
	}

	GWindow::GWindow(const Attribute& attribute)
	{
		this->InitializeWindow(attribute);
	}

	GWindow::~GWindow()
	{
		this->DestroyWindow();
		this->WindowHandle = null;
	}

	void GWindow::InitializeWindow(const Attribute& attribute)
	{
		this->WindowAttribute = attribute;
		GUARDIAN_CHECK_VALUE(this->WindowAttribute.WindowTitle.size());

		WNDCLASSEX WindowClass;
		GUARDIAN_CLEAR_MEMORY(WindowClass);
		WindowClass.cbSize = sizeof(WNDCLASSEXA);
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		WindowClass.hCursor = LoadCursor(null, IDC_ARROW);
		WindowClass.hInstance = GetModuleHandle(null);
		WindowClass.lpfnWndProc = InitializeWindowMessageProcess;
		std::wstring WindowClassName = GConverter::StringToWideString(this->WindowAttribute.WindowTitle) + L" Class";
		WindowClass.lpszClassName = WindowClassName.c_str();
		WindowClass.lpszMenuName = null;
		WindowClass.style = CS_CLASSDC;

		if (!RegisterClassEx(&WindowClass))
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}

		RECT WindowRectangle;
		GUARDIAN_CLEAR_MEMORY(WindowRectangle);
		DWORD Style = 0;
		switch (this->WindowAttribute.WindowStyle)
		{
			case GE_STYLE_DEFAULTWINDOW:
			{
				Style = WS_OVERLAPPED | WS_SIZEBOX;

				int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
				int WindowWidth = (int)(ScreenWidth * 0.8f);
				int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
				int WindowHeight = (int)(ScreenHeight* 0.8f);

				WindowRectangle.left = (int)((ScreenWidth - WindowWidth) / 2.0f);
				WindowRectangle.right = (int)(ScreenWidth - ((ScreenWidth - WindowWidth) / 2.0f));
				WindowRectangle.top = (int)((ScreenHeight - WindowHeight) / 2.0f);
				WindowRectangle.bottom = (int)(ScreenHeight - ((ScreenHeight - WindowHeight) / 2.0f));

				if (!AdjustWindowRect(&WindowRectangle, Style, false))
				{
					throw GUARDIAN_LAST_WINDOW_EXCEPTION();
				}
			
				break;
			}

			case GE_STYLE_BORDERLESSWINDOW:
			{
				Style = WS_POPUPWINDOW;

				int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
				int WindowWidth = (int)(ScreenWidth * 0.8f);
				int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
				int WindowHeight = (int)(ScreenHeight * 0.8f);

				WindowRectangle.left = (int)((ScreenWidth - WindowWidth) / 2.0f);
				WindowRectangle.right = (int)(ScreenWidth - ((ScreenWidth - WindowWidth) / 2.0f));
				WindowRectangle.top = (int)((ScreenHeight - WindowHeight) / 2.0f);
				WindowRectangle.bottom = (int)(ScreenHeight - ((ScreenHeight - WindowHeight) / 2.0f));

				break;
			}

			case GE_STYLE_FULLSCREEN:
			{
				Style = WS_POPUPWINDOW | WS_VISIBLE;

				WindowRectangle.left = 0;
				WindowRectangle.right = GetSystemMetrics(SM_CXSCREEN);
				WindowRectangle.top = 0;
				WindowRectangle.bottom = GetSystemMetrics(SM_CYSCREEN);

				break;
			}
		}

		this->WindowHandle = CreateWindowEx(0,
											 WindowClassName.c_str(),
											 GConverter::StringToWideString(this->WindowAttribute.WindowTitle).c_str(),
											 Style,
											 WindowRectangle.left,
											 WindowRectangle.top,
											 WindowRectangle.right - WindowRectangle.left,
											 WindowRectangle.bottom - WindowRectangle.top,
											 null,
											 null,
											 GetModuleHandle(null),
											 this);
		if (!this->WindowHandle)
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}

		GUARDIAN_CLEAR_MEMORY(this->WindowMessage);
	}

	void GWindow::DisplayWindow()
	{
		if (!::UpdateWindow(this->WindowHandle))
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}
		ShowWindow(this->WindowHandle, SW_SHOW);

		SetForegroundWindow(this->WindowHandle);
		SetFocus(this->WindowHandle);

		this->IsWindowRunning = true;
	}

	void GWindow::UpdateWindow()
	{
		while (PeekMessage(&this->WindowMessage, null, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&this->WindowMessage);

			DispatchMessage(&this->WindowMessage);
		}

		this->IsWindowRunning = this->WindowMessage.message != WM_QUIT;
	}

	void GWindow::DestroyWindow()
	{
		::DestroyWindow(this->WindowHandle);
		UnregisterClass(GConverter::StringToWideString(this->WindowAttribute.WindowTitle + " Class").c_str(), GetModuleHandle(null));
		GUARDIAN_CLEAR_MEMORY(this->WindowMessage);
	}

	const bool& GWindow::GetWindowRunning() const noexcept
	{
		return this->IsWindowRunning;
	}

	const GWindow::Handle& GWindow::GetWindowHandle() const noexcept
	{
		return this->WindowHandle;
	}

	const GWindow::Attribute& GWindow::GetWindowAttribute() const noexcept
	{
		return this->WindowAttribute;
	}

	const std::string& GWindow::GetWindowTitle() const noexcept
	{
		return this->WindowAttribute.WindowTitle;
	}

	const GWindow::Style& GWindow::GetWindowStyle() const noexcept
	{
		return this->WindowAttribute.WindowStyle;
	}

	const GWindow::Theme& GWindow::GetWindowTheme() const noexcept
	{
		return this->WindowAttribute.WindowTheme;
	}

	LRESULT WINAPI GWindow::InitializeWindowMessageProcess(Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			const CREATESTRUCTW* const Creation = (CREATESTRUCTW*)lParam;
			GWindow* const window = (GWindow*)Creation->lpCreateParams;

			SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)window);
			SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)&GWindow::StaticWindowMessageProcess);

			return window->ProcessWindowMessage(handle, message, wParam, lParam);
		}

		return DefWindowProc(handle, message, wParam, lParam);
	}

	LRESULT WINAPI GWindow::StaticWindowMessageProcess(Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		GWindow* const window = (GWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);

		return window->ProcessWindowMessage(handle, message, wParam, lParam);
	}

	LRESULT WINAPI GWindow::ProcessWindowMessage(Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static bool IsNCActive = false;

		switch (message)
		{
			case WM_CLOSE:
			{
				PostQuitMessage(0);

				return 0;
				break;
			}

			case WM_NCCREATE:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				SetTimer(handle, 1, 100, null);
				SetWindowTheme(handle, L"", L"");

				DWORD Style = GetWindowLong(handle, GCL_STYLE);
				SetClassLong(handle, GCL_STYLE, Style | CS_DROPSHADOW);

				return 1;
				break;
			}

			case WM_TIMER:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				SetWindowPos(handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);
				SendMessage(handle, WM_PAINT, 0, 0);

				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_NCACTIVATE:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				IsNCActive = LOWORD(wParam) != WA_INACTIVE;

				return 1;
			}

			case WM_NCPAINT:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				switch (this->WindowAttribute.WindowTheme)
				{
				case GWindow::GE_THEME_DARK:
				{
					this->DrawDarkCaption(IsNCActive, handle, message, wParam, lParam);
					break;
				}
				case GWindow::GE_THEME_LIGHT:
				{
					this->DrawLightCaption(IsNCActive, handle, message, wParam, lParam);
					break;
				}
				case GWindow::GE_THEME_BLUE:
				{
					this->DrawBlueCaption(IsNCActive, handle, message, wParam, lParam);
					break;
				}
				}

				return 0;
				break;
			}

			case WM_GETMINMAXINFO:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				MINMAXINFO* minmax = (MINMAXINFO*)lParam;

				RECT WorkArea = {};
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
				minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
				minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
				minmax->ptMaxPosition.x = WorkArea.left;
				minmax->ptMaxPosition.y = WorkArea.top;
				minmax->ptMinTrackSize.x = 1000;
				minmax->ptMinTrackSize.y = 600;

				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_NCLBUTTONDOWN:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				POINT pt;
				GetCursorPos(&pt);

				RECT rect;
				GetWindowRect(handle, &rect);

				pt.x -= rect.left;
				pt.y -= rect.top;

				SIZE size{ rect.right - rect.left, rect.bottom - rect.top };

				rect = RECT{ size.cx - 50, 0, size.cx - 50 + 50, 30 };

				if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
				{
					PostQuitMessage(0);
					return 0;
					break;
				}

				rect = RECT{ size.cx - 100, 0, size.cx - 100 + 50, 30 };

				if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
				{
					WINDOWPLACEMENT WindowPosition = {};
					GetWindowPlacement(handle, &WindowPosition);

					if (WindowPosition.showCmd == SW_MAXIMIZE)
					{
						ShowWindow(handle, SW_NORMAL);
					}
					else
					{
						ShowWindow(handle, SW_MAXIMIZE);
					}
				}

				rect = RECT{ size.cx - 150, 0, size.cx - 150 + 50, 30 };

				if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
				{
					ShowWindow(handle, SW_MINIMIZE);
				}

				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_EXITSIZEMOVE:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				RECT rect;
				GetWindowRect(handle, &rect);
				RECT WorkArea = {};
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
				if (rect.top < WorkArea.top + 10)
				{
					WINDOWPLACEMENT WindowPosition = {};
					GetWindowPlacement(handle, &WindowPosition);

					if (WindowPosition.showCmd == SW_MAXIMIZE)
					{
						ShowWindow(handle, SW_NORMAL);
					}
					else
					{
						ShowWindow(handle, SW_MAXIMIZE);
					}
				}

				return 0;
				break;
			}

			case WM_NCLBUTTONDBLCLK:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				WINDOWPLACEMENT WindowPosition = {};
				GetWindowPlacement(handle, &WindowPosition);

				if (WindowPosition.showCmd == SW_MAXIMIZE)
				{
					ShowWindow(handle, SW_NORMAL);
				}
				else
				{
					ShowWindow(handle, SW_MAXIMIZE);
				}

				return 0;
				break;
			}
		}

		return DefWindowProc(handle, message, wParam, lParam);
	}

	void GWindow::DrawDarkCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = GetDCEx(handle, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN | 0x00010000);

		RECT rect;
		GetWindowRect(handle, &rect);

		SIZE size{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMem);

		HBRUSH brush = CreateSolidBrush(RGB(20, 20, 20));

		RECT newRect{ 0, 0, size.cx, size.cy };
		FillRect(hdc, &newRect, brush);

		if (isActive)
		{
			brush = CreateSolidBrush(RGB(63, 29, 0));
			FrameRect(hdc, &newRect, brush);
		}

		RECT TextRect = RECT{ 0, 0, size.cx, 30 };
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, isActive ? RGB(255, 255, 255) : RGB(100, 100, 100));
		DrawText(hdc, GConverter::StringToWideString(this->WindowAttribute.WindowTitle).c_str(),
			(int)GConverter::StringToWideString(this->WindowAttribute.WindowTitle).size(), &TextRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		POINT pt;
		GetCursorPos(&pt);

		RECT WindowRect;
		GetWindowRect(handle, &WindowRect);

		pt.x -= WindowRect.left;
		pt.y -= WindowRect.top;

		rect = RECT{ size.cx - 50, 0, size.cx - 50 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"X", (int)wcslen(L"X"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 100, 0, size.cx - 100 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"口", (int)wcslen(L"口"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 150, 0, size.cx - 150 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"-", (int)wcslen(L"-"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		DeleteObject(brush);

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);

		SelectObject(hdc, hOld);
		DeleteObject(hbmMem);

		ReleaseDC(handle, hdc);
	}

	void GWindow::DrawLightCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = GetDCEx(handle, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN | 0x00010000);

		RECT rect;
		GetWindowRect(handle, &rect);

		SIZE size{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMem);

		HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));

		RECT newRect{ 0, 0, size.cx, size.cy };
		FillRect(hdc, &newRect, brush);

		if (isActive)
		{
			brush = CreateSolidBrush(RGB(163, 129, 0));
			FrameRect(hdc, &newRect, brush);
		}

		RECT TextRect = RECT{ 0, 0, size.cx, 30 };
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, isActive ? RGB(0, 0, 0) : RGB(100, 100, 100));
		DrawText(hdc, GConverter::StringToWideString(this->WindowAttribute.WindowTitle).c_str(),
			(int)GConverter::StringToWideString(this->WindowAttribute.WindowTitle).size(), &TextRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		POINT pt;
		GetCursorPos(&pt);

		RECT WindowRect;
		GetWindowRect(handle, &WindowRect);

		pt.x -= WindowRect.left;
		pt.y -= WindowRect.top;

		rect = RECT{ size.cx - 50, 0, size.cx - 50 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"X", (int)wcslen(L"X"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 100, 0, size.cx - 100 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"口", (int)wcslen(L"口"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 150, 0, size.cx - 150 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"-", (int)wcslen(L"-"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		DeleteObject(brush);

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);

		SelectObject(hdc, hOld);
		DeleteObject(hbmMem);

		ReleaseDC(handle, hdc);
	}

	void GWindow::DrawBlueCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = GetDCEx(handle, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN | 0x00010000);

		RECT rect;
		GetWindowRect(handle, &rect);

		SIZE size{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMem);

		HBRUSH brush = CreateSolidBrush(RGB(0, 50, 200));

		RECT newRect{ 0, 0, size.cx, size.cy };
		FillRect(hdc, &newRect, brush);

		if (isActive)
		{
			brush = CreateSolidBrush(RGB(0, 50, 163));
			FrameRect(hdc, &newRect, brush);
		}

		RECT TextRect = RECT{ 0, 0, size.cx, 30 };
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, isActive ? RGB(0, 0, 0) : RGB(100, 100, 100));
		DrawText(hdc, GConverter::StringToWideString(this->WindowAttribute.WindowTitle).c_str(),
			(int)GConverter::StringToWideString(this->WindowAttribute.WindowTitle).size(), &TextRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		POINT pt;
		GetCursorPos(&pt);

		RECT WindowRect;
		GetWindowRect(handle, &WindowRect);

		pt.x -= WindowRect.left;
		pt.y -= WindowRect.top;

		rect = RECT{ size.cx - 50, 0, size.cx - 50 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"X", (int)wcslen(L"X"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 100, 0, size.cx - 100 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"口", (int)wcslen(L"口"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rect = RECT{ size.cx - 150, 0, size.cx - 150 + 50, 30 };

		if (rect.left < pt.x && rect.right > pt.x && rect.top < pt.y && rect.bottom > pt.y)
		{
			HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
			FillRect(hdc, &rect, brush);

			DeleteObject(brush);
		}

		DrawText(hdc, L"-", (int)wcslen(L"-"), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		DeleteObject(brush);

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);

		SelectObject(hdc, hOld);
		DeleteObject(hbmMem);

		ReleaseDC(handle, hdc);
	}
}
