#include "GuardianApplication.h"
#include "../GuardianEngine.h"

namespace guardian
{
	std::unique_ptr<GuardianApplication> GuardianApplication::ApplicationInstance = std::make_unique<GuardianApplication>();


	GuardianApplication::GuardianApplication()
	{
		this->ApplicationWindow = std::make_shared<GuardianWindow>();
		this->ApplicationEventProcesser = std::make_unique<GuardianEventProcesser>();
	}

	GuardianApplication::~GuardianApplication()
	{

	}

	void GuardianApplication::InitializeApplication(const GuardianWindowProperties& windowProperties)
	{
		this->ApplicationWindow = GuardianWindow::CreateNewWindow(
			windowProperties, GuardianApplication::ApplicationMessageProcessFunction);

		this->ApplicationEventProcesser->OnEvent<GuardianWindowCloseEvent>([this](const GuardianWindowCloseEvent& event)
		{
			if (this->ApplicationWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->ApplicationWindow->IsWindowRunning = false;
			}
		});

		this->ApplicationEventProcesser->OnEvent<GuardianWindowResizeEvent>([this](const GuardianWindowResizeEvent& event)
		{
			if (this->ApplicationWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->ApplicationWindow->WindowGraphicsContext->UpdateGraphicsResolution(event.ResizeWidth, event.ResizeHeight);
			}

			if (GuardianRenderer::RendererCamera.get())
			{
				GuardianRenderer::UpdateProjectionAspect((float)event.ResizeWidth, (float)event.ResizeHeight);
			}
		});
	}

	void GuardianApplication::DisplayWindow()
	{
		this->ApplicationWindow->DisplayWindow();
	}

	void GuardianApplication::UpdateApplication() noexcept
	{
		this->ApplicationWindow->UpdateWinow();
	}

	void GuardianApplication::BeginRendering(const GVector3& clearColor)
	{
		this->ApplicationWindow->WindowGraphicsContext->BeginRendering(clearColor);
	}

	void GuardianApplication::EndUpRendering(int syncInterval)
	{
		this->ApplicationWindow->WindowGraphicsContext->EndUpRendering(syncInterval);
	}

	const bool GuardianApplication::IsApplicationRunning() const noexcept
	{
		return this->ApplicationWindow->GetIsWindowRunning();
	}

	std::shared_ptr<GuardianWindow> GuardianApplication::GetApplicationWindow() noexcept
	{
		return this->ApplicationWindow;
	}

	GWindowHandle GuardianApplication::GetApplicationWindowHandle() noexcept
	{
		return this->ApplicationWindow->WindowHandle;
	}

	std::shared_ptr<GuardianGraphics> GuardianApplication::GetApplicationGraphicsContext() noexcept
	{
		return this->ApplicationWindow->GetWindowGraphicsContext();
	}

	LRESULT GuardianApplication::ApplicationMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (GuardianEngine::EngineInstance->EngineProgram->EditorMessageProcess(hWnd, msg, wParam, lParam))
		{
			return true;
		}

		switch (msg)
		{
			case WM_CLOSE:
			{
				GuardianWindowCloseEvent windowClosing(hWnd);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(windowClosing);
				return 0;
				break;
			}

			case WM_SIZE:
			{
				if (wParam == SIZE_MINIMIZED)
				{
					return 0;
					break;
				}

				GuardianWindowResizeEvent windowResizing(hWnd, (int)LOWORD(lParam), (int)HIWORD(lParam));
				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(windowResizing);
				return 0;
				break;
			}

			default:
			{
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}
		}
	}
}
