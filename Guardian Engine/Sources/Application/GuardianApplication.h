#ifndef _GE_GUARDIANAPPLICATION_H_
#define _GE_GUARDIANAPPLICATION_H_
#include "Window/GuardianWindow.h"

namespace guardian
{
	class GUARDIAN_API GuardianApplication
	{
	public:
		GuardianApplication();
		GuardianApplication(const GuardianApplication&) = delete;
		~GuardianApplication();

		const bool IsApplicationRunning() const noexcept;
		std::shared_ptr<GuardianWindow> GetApplicationWindow() noexcept;
		GWindowHandle GetApplicationWindowHandle() noexcept;
		std::shared_ptr<GuardianGraphics> GetApplicationGraphicsContext() noexcept;

		static std::unique_ptr<GuardianApplication> ApplicationInstance;

	private:
		void InitializeApplication(const GuardianWindowProperties& windowProperties);

		void DisplayWindow();
		void UpdateApplication() noexcept;

		void BeginRendering(const GVector3& clearColor = GVector3(0.2f, 0.3f, 0.3f));
		void EndUpRendering(int syncInterval = 1);

		static LRESULT CALLBACK ApplicationMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		std::shared_ptr<GuardianWindow> ApplicationWindow;
		std::unique_ptr<GuardianEventProcesser> ApplicationEventProcesser;
		
		friend class GuardianEngine;
	};
}

#endif