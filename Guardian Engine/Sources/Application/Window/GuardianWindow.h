#ifndef _GE_GUARDIANWINDOW_H_
#define _GE_GUARDIANWINDOW_H_
#include "../ResourceSystem/GuardianResourceSystem.h"

namespace guardian
{
	struct GUARDIAN_API GuardianWindowProperties
	{
	public:
		GuardianWindowProperties() : 
			GuardianWindowProperties(1536, 864, "Guardian Window")
		{
			
		}
		GuardianWindowProperties(const GuardianWindowProperties&) = default;
		GuardianWindowProperties(int width, int height, const GString& title)
		{
			this->SetWindowResolution(width, height);
			this->SetWindowTitle(title);
			this->IsWindowFullscreen = false;
		}

		void SetWindowFullscreen(const GString& title)
		{
			this->WindowWidth = GetSystemMetrics(SM_CXSCREEN);
			this->WindowHeight = GetSystemMetrics(SM_CYSCREEN);
			this->WindowPositionX = 0;
			this->WindowPositionY = 0;
			this->WindowTitle = title;
			this->IsWindowFullscreen = true;
		}

		void SetWindowResolution(int width, int height)
		{
			this->WindowWidth = width;
			this->WindowHeight = height;
			this->WindowPositionX = (GetSystemMetrics(SM_CXSCREEN) - this->WindowWidth) / 2;
			this->WindowPositionY = (GetSystemMetrics(SM_CYSCREEN) - this->WindowHeight) / 2;
		}

		void SetWindowTitle(const GString& title)
		{
			this->WindowTitle = title;
			this->WindowClassName = this->WindowTitle + " Class";
		}

		const int GetWidth() const noexcept
		{
			return this->WindowWidth;
		}

		const int GetHeight() const noexcept
		{
			return this->WindowHeight;
		}

	private:
		int WindowWidth;
		int WindowHeight;
		GString WindowTitle;
		GString WindowClassName;
		int WindowPositionX;
		int WindowPositionY;
		bool IsWindowFullscreen;

		friend class GuardianWindow;
	};

	class GUARDIAN_API GuardianWindow
	{
	public:
		GuardianWindow();
		GuardianWindow(const GuardianWindow&) = default;
		GuardianWindow(const GuardianWindowProperties& properties, WNDPROC windowProcessFunction);
		~GuardianWindow();

		void InitializeWindow(const GuardianWindowProperties& properties, WNDPROC windowProcessFunction);

		void DisplayWindow();
		void UpdateWindowMessage() noexcept;

		const bool GetIsWindowRunning() const noexcept;
		const GWindowHandle& GetWindowHandle() const noexcept;
		std::shared_ptr<GuardianGraphics> GetWindowGraphicsContext() noexcept;
		const GuardianWindowProperties& GetWindowProperties() const noexcept;

		static std::shared_ptr<GuardianWindow> CreateNewWindow(
			const GuardianWindowProperties& properties, WNDPROC windowProcessFunction);

	private:
		GuardianWindowProperties WindowProperties;
		GWindowHandle WindowHandle;

		std::shared_ptr<GuardianGraphics> WindowGraphicsContext;

		MSG WindowMessage;
		bool IsWindowRunning;

		friend class GuardianApplication;
	};
}

#endif