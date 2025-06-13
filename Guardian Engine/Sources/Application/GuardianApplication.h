#ifndef _GE_GUARDIANAPPLICATION_H_
#define _GE_GUARDIANAPPLICATION_H_
#include "Window/GuardianWindow.h"

namespace GE
{
	class GUARDIAN_API GuardianApplication
	{
	public:
		struct CommandLineArgument
		{
			bool IsEmpty() const
			{
				return this->ArgumentList.empty();
			}
			static const int SCENE_PATH_ARGUMENT_INDEX = 1;

			std::vector<GString> ArgumentList;
		};

	public:
		GuardianApplication();
		GuardianApplication(const GuardianApplication&) = delete;
		~GuardianApplication();

		const bool IsApplicationRunning() const noexcept;

		std::shared_ptr<GuardianWindow> GetCreatorWindow() noexcept;
		GWindowHandle GetCreatorWindowHandle() noexcept;
		std::shared_ptr<GuardianWindow> GetMainWindow() noexcept;
		GWindowHandle GetMainWindowHandle() noexcept;

		std::shared_ptr<GuardianGraphics> GetCreatorGraphicsContext() noexcept;
		std::shared_ptr<GuardianGraphics> GetMainGraphicsContext() noexcept;

		static std::unique_ptr<GuardianApplication> ApplicationInstance;

		static void SetCommandLineArgument(int argc, char* argv[]);
		static CommandLineArgument ApplicationCommandLineArgument;

	private:
		void InitializeApplication(const GuardianWindowProperties& windowProperties);

		void LaunchStartupWindow();

		void DisplayCreatorWindow();
		void DisplayMainWindow();
		void UpdateApplication() noexcept;

		void EndUpRendering(int syncInterval = 1);

		static LRESULT CALLBACK StartupWindowMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK CreatorWindowMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK ApplicationMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		std::shared_ptr<GuardianWindow> StartupWindow;
		std::shared_ptr<GuardianWindow> CreatorWindow;
		std::shared_ptr<GuardianWindow> MainWindow;
		std::unique_ptr<GuardianEventProcesser> ApplicationEventProcesser;
		
		friend class GuardianEngine;
	};
}

#endif