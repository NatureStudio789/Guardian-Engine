#include "GWindow.h"
#include "../../GEngine.h"

namespace GE
{
	GWindow::GWindow()
	{
		this->WindowHandle = null;
		GUARDIAN_CLEAR_MEMORY(this->WindowArea);
		this->WindowAttribute = Attribute();

		this->WindowEventProcesser = std::make_shared<GEventProcesser>();
		this->IsWindowRunning = false;
		GUARDIAN_CLEAR_MEMORY(this->WindowMessage);
	}

	GWindow::GWindow(const GWindow& other)
	{
		this->WindowHandle = other.WindowHandle;
		this->WindowArea = other.WindowArea;
		this->WindowAttribute = other.WindowAttribute;
		this->WindowMessage = other.WindowMessage;

		this->WindowEventProcesser = other.WindowEventProcesser;
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

		GUARDIAN_CLEAR_MEMORY(WindowArea);
		DWORD Style = 0;
		switch (this->WindowAttribute.WindowStyle)
		{
			case GE_STYLE_DEFAULTWINDOW:
			{
				Style = WS_OVERLAPPEDWINDOW;

				int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
				int WindowWidth = (int)(ScreenWidth * 0.8f);
				int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
				int WindowHeight = (int)(ScreenHeight* 0.8f);

				WindowArea.left = (int)((ScreenWidth - WindowWidth) / 2.0f);
				WindowArea.right = (int)(ScreenWidth - ((ScreenWidth - WindowWidth) / 2.0f));
				WindowArea.top = (int)((ScreenHeight - WindowHeight) / 2.0f);
				WindowArea.bottom = (int)(ScreenHeight - ((ScreenHeight - WindowHeight) / 2.0f));

				if (!AdjustWindowRect(&WindowArea, Style, false))
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

				WindowArea.left = (int)((ScreenWidth - WindowWidth) / 2.0f);
				WindowArea.right = (int)(ScreenWidth - ((ScreenWidth - WindowWidth) / 2.0f));
				WindowArea.top = (int)((ScreenHeight - WindowHeight) / 2.0f);
				WindowArea.bottom = (int)(ScreenHeight - ((ScreenHeight - WindowHeight) / 2.0f));

				break;
			}

			case GE_STYLE_FULLSCREEN:
			{
				Style = WS_POPUPWINDOW | WS_VISIBLE;

				WindowArea.left = 0;
				WindowArea.right = GetSystemMetrics(SM_CXSCREEN);
				WindowArea.top = 0;
				WindowArea.bottom = GetSystemMetrics(SM_CYSCREEN);

				break;
			}
		}

		this->WindowHandle = CreateWindowEx(0,
											 WindowClassName.c_str(),
											 GConverter::StringToWideString(this->WindowAttribute.WindowTitle).c_str(),
											 Style,
											 WindowArea.left,
											 WindowArea.top,
											 WindowArea.right - WindowArea.left,
											 WindowArea.bottom - WindowArea.top,
											 null,
											 null,
											 GetModuleHandle(null),
											 this);
		if (!this->WindowHandle)
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}

		GUARDIAN_CLEAR_MEMORY(this->WindowMessage);

		this->WindowEventProcesser = std::make_shared<GEventProcesser>();
		this->WindowEventProcesser->OnEvent<GWindowCloseEvent>([=](const GWindowCloseEvent& event)
		{
			if (event.WindowHandle == this->WindowHandle)
			{
				this->IsWindowRunning = false;
			}
		});
	}

	void GWindow::DisplayWindow()
	{
		if (!::UpdateWindow(this->WindowHandle))
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}
		ShowWindow(this->WindowHandle, SW_SHOW);

		this->IsWindowRunning = true;
	}

	void GWindow::UpdateWindow()
	{
		while (PeekMessage(&this->WindowMessage, null, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&this->WindowMessage);

			DispatchMessage(&this->WindowMessage);
		}
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

	const RECT& GWindow::GetWindowArea() const noexcept
	{
		return this->WindowArea;
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
		if (GEngine::Instance->EngineProgram->ProcessWindowMessage(handle, message, wParam, lParam))
		{
			return 0;
		}

		static bool IsNCActive = false;
		static bool IsBeingResized = false;
		static bool IsMaximized = false;
		static bool IsMinimized = false;
		static int WindowWidth = 0;
		static int WindowHeight = 0;

		switch (message)
		{
			case WM_CLOSE:
			{
				GWindowCloseEvent event(handle);
				GEventDispatcher::DispatcherInstance->DispatchEvent(event);

				return 0;
				break;
			}

			/*case WM_NCCREATE:
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
			}*/

			case WM_GETMINMAXINFO:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				MINMAXINFO* minmax = (MINMAXINFO*)lParam;

				/*RECT WorkArea = {};
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
				minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
				minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
				minmax->ptMaxPosition.x = WorkArea.left;
				minmax->ptMaxPosition.y = WorkArea.top;*/
				minmax->ptMinTrackSize.x = 1000;
				minmax->ptMinTrackSize.y = 600;

				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			/*case WM_NCLBUTTONDOWN:
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
					SendMessage(handle, WM_CLOSE, 0, 0);

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
			}*/

			case WM_ENTERSIZEMOVE:
			{
				IsBeingResized = true;

				return 0;
				break;
			}

			case WM_EXITSIZEMOVE:
			{
				if (this->WindowAttribute.WindowStyle != GE_STYLE_DEFAULTWINDOW)
				{
					break;
				}

				/*RECT rect;
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
				}*/

				IsBeingResized = false;
				GWindowResizeEvent windowResizing(handle, WindowWidth, WindowHeight);
				GEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);

				return 0;
				break;
			}

			/*case WM_NCLBUTTONDBLCLK:
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
			}*/

			case WM_MOVE:
			{
				GWindowMoveEvent windowMoving(handle, (int)LOWORD(lParam), (int)HIWORD(lParam));

				GEventDispatcher::DispatcherInstance->DispatchEvent(windowMoving);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_ACTIVATE:
			{
				if (LOWORD(lParam) == WA_ACTIVE || LOWORD(lParam) == WA_CLICKACTIVE)
				{
					GWindowFocusEvent windowFocusing(handle);

					GEventDispatcher::DispatcherInstance->DispatchEvent(windowFocusing);
				}
				else if (LOWORD(lParam) == WA_INACTIVE)
				{
					GWindowLostFocusEvent windowLostFocusing(handle);

					GEventDispatcher::DispatcherInstance->DispatchEvent(windowLostFocusing);
				}
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_SIZE:
			{
				WindowWidth = (int)LOWORD(lParam);
				WindowHeight = (int)HIWORD(lParam);

				if (wParam == SIZE_MINIMIZED)
				{
					IsMinimized = true;
					IsMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					IsMinimized = false;
					IsMaximized = true;

					GWindowResizeEvent windowResizing(handle, (int)LOWORD(lParam), (int)HIWORD(lParam));
					GEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (IsMinimized)
					{
						IsMinimized = false;
						GWindowResizeEvent windowResizing(handle, (int)LOWORD(lParam), (int)HIWORD(lParam));
						GEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);
					}
					else if (IsMaximized)
					{
						IsMaximized = false;

						GWindowResizeEvent windowResizing(handle, (int)LOWORD(lParam), (int)HIWORD(lParam));
						GEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);
					}
					else if (IsBeingResized)
					{
						// Can't Resize!!!
					}
					else
					{
						GWindowResizeEvent windowResizing(handle, (int)LOWORD(lParam), (int)HIWORD(lParam));
						GEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);
					}
				}

				return 0;
				break;
			}

			case WM_KEYDOWN:
			{
				GKeyPressEvent keyPressing((UINT)wParam);

				GEventDispatcher::DispatcherInstance->DispatchEvent(keyPressing);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_KEYUP:
			{
				GKeyReleaseEvent keyReleasing((UINT)wParam);

				GEventDispatcher::DispatcherInstance->DispatchEvent(keyReleasing);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_CHAR:
			{
				GCharEvent charEvent((UCHAR)wParam);

				GEventDispatcher::DispatcherInstance->DispatchEvent(charEvent);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_MOUSEMOVE:
			{
				GMouseMoveEvent mouseMoving((int)LOWORD(lParam), (int)HIWORD(lParam));

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseMoving);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_LBUTTONDOWN:
			{
				GMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_LEFT);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_MBUTTONDOWN:
			{
				GMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_MIDDLE);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_RBUTTONDOWN:
			{
				GMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_RIGHT);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_LBUTTONUP:
			{
				GMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_LEFT);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}
			case WM_MBUTTONUP:
			{
				GMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_MIDDLE);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_RBUTTONUP:
			{
				GMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_RIGHT);

				GEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(handle, message, wParam, lParam);
				break;
			}

			case WM_INPUT:
			{
				UINT DataSize = 0;
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, null, &DataSize, sizeof(RAWINPUTHEADER));

				if (DataSize > 0)
				{
					std::unique_ptr<BYTE[]> RawData = std::make_unique<BYTE[]>(DataSize);
					if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
						RID_INPUT, RawData.get(), &DataSize, sizeof(RAWINPUTHEADER)) == DataSize)
					{
						RAWINPUT* Raw = reinterpret_cast<RAWINPUT*>(RawData.get());

						if (Raw->header.dwType == RIM_TYPEMOUSE)
						{
							GMouseRawMoveEvent mouseRawMoving((float)Raw->data.mouse.lLastX, (float)Raw->data.mouse.lLastY);

							GEventDispatcher::DispatcherInstance->DispatchEvent(mouseRawMoving);
						}
					}
				}

				return DefWindowProc(handle, message, wParam, lParam);
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
