#include "GuardianWindow.h"

namespace GE
{
	GuardianWindow::GuardianWindow()
	{
		this->WindowHandle = null;
		this->WindowProperties = GuardianWindowProperties();
		this->WindowMessage = { 0 };
		this->IsWindowRunning = false;
		this->WindowGraphicsContext = std::make_shared<GuardianGraphics>();
	}

	GuardianWindow::GuardianWindow(const GuardianWindowProperties& properties, WNDPROC windowProcessFunction)
	{
		this->InitializeWindow(properties, windowProcessFunction);
	}

	GuardianWindow::~GuardianWindow()
	{
		UnregisterClass(GuardianConverter::StringToWideString(this->WindowProperties.WindowClassName).c_str(), GetModuleHandle(null));
	}

	void GuardianWindow::InitializeWindow(const GuardianWindowProperties& properties, WNDPROC windowProcessFunction)
	{
		this->IsWindowRunning = false;
		this->WindowProperties = properties;

		WNDCLASSEX WindowClass;
		ZeroMemory(&WindowClass, sizeof(WNDCLASSEX));
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		WindowClass.hCursor = LoadCursor(null, IDC_ARROW);
		WindowClass.hInstance = GetModuleHandle(null);
		WindowClass.lpfnWndProc = windowProcessFunction;
		GWideString WideClassName = GuardianConverter::StringToWideString(this->WindowProperties.WindowClassName);
		WindowClass.lpszClassName = WideClassName.c_str();
		WindowClass.lpszMenuName = null;
		WindowClass.style = CS_CLASSDC;

		if (!RegisterClassEx(&WindowClass))
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}

		DWORD Style = 0;
		RECT WindowRectangle = { 0 };
		WindowRectangle.left = this->WindowProperties.WindowPositionX;
		WindowRectangle.top = this->WindowProperties.WindowPositionY;
		WindowRectangle.right = this->WindowProperties.WindowPositionX + this->WindowProperties.WindowWidth;
		WindowRectangle.bottom = this->WindowProperties.WindowPositionY + this->WindowProperties.WindowHeight;
		if (this->WindowProperties.IsWindowFullscreen && !this->WindowProperties.HasBorder)
		{
			Style = WS_POPUPWINDOW | WS_VISIBLE;
		}
		else if (!this->WindowProperties.IsWindowFullscreen && !this->WindowProperties.HasBorder)
		{
			Style = WS_POPUP;
		}
		else
		{
			Style = WS_OVERLAPPEDWINDOW;
			if (!AdjustWindowRect(&WindowRectangle, Style, false))
			{
				throw GUARDIAN_LAST_WINDOW_EXCEPTION();
			}
		}
		this->WindowHandle = CreateWindowExA(0, this->WindowProperties.WindowClassName.c_str(),
			this->WindowProperties.WindowTitle.c_str(), Style, WindowRectangle.left, WindowRectangle.top,
			WindowRectangle.right - WindowRectangle.left, WindowRectangle.bottom - WindowRectangle.top,
			null, 0, GetModuleHandle(null), 0);
		if (!this->WindowHandle)
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}

		this->WindowMessage = { 0 };

		this->WindowGraphicsContext = GuardianGraphics::CreateNewGraphics(this->WindowHandle, 
			this->WindowProperties.WindowWidth, this->WindowProperties.WindowHeight, this->WindowProperties.IsWindowFullscreen);
	}

	void GuardianWindow::DisplayWindow()
	{
		if (!UpdateWindow(this->WindowHandle))
		{
			throw GUARDIAN_LAST_WINDOW_EXCEPTION();
		}
		ShowWindow(this->WindowHandle, SW_SHOW);

		SetForegroundWindow(this->WindowHandle);
		SetFocus(this->WindowHandle);

		this->IsWindowRunning = true;
	}

	void GuardianWindow::UpdateWindowMessage() noexcept
	{
		while (PeekMessage(&this->WindowMessage, null, null, null, PM_REMOVE))
		{
			TranslateMessage(&this->WindowMessage);

			DispatchMessage(&this->WindowMessage);
		}
	}

	void GuardianWindow::DestroyWindow()
	{
		::DestroyWindow(this->WindowHandle);
		UnregisterClass(GuardianConverter::StringToWideString(this->WindowProperties.WindowClassName).c_str(), GetModuleHandle(null));
	}

	const bool GuardianWindow::GetIsWindowRunning() const noexcept
	{
		return this->IsWindowRunning;
	}

	const GWindowHandle& GuardianWindow::GetWindowHandle() const noexcept
	{
		return this->WindowHandle;
	}

	std::shared_ptr<GuardianGraphics> GuardianWindow::GetWindowGraphicsContext() noexcept
	{
		return this->WindowGraphicsContext;
	}

	const GuardianWindowProperties& GuardianWindow::GetWindowProperties() const noexcept
	{
		return this->WindowProperties;
	}

	std::shared_ptr<GuardianWindow> GuardianWindow::CreateNewWindow(
		const GuardianWindowProperties& properties, WNDPROC windowProcessFunction)
	{
		return std::make_shared<GuardianWindow>(properties, windowProcessFunction);
	}
}
