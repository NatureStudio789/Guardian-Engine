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

		GuardianInput::InitializeInput();

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
			return 0;
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

			case WM_SYSCOMMAND:
			{
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_MOVE:
			{
				GuardianWindowMoveEvent windowMoving(hWnd, (int)LOWORD(lParam), (int)HIWORD(lParam));

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(windowMoving);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_ACTIVATE:
			{
				if (LOWORD(lParam) == WA_ACTIVE || LOWORD(lParam) == WA_CLICKACTIVE)
				{
					GuardianWindowFocusEvent windowFocusing(hWnd);

					GuardianEventDispatcher::DispatcherInstance->DispatchEvent(windowFocusing);
				}
				else if (LOWORD(lParam) == WA_INACTIVE)
				{
					GuardianWindowLostFocusEvent windowLostFocusing(hWnd);

					GuardianEventDispatcher::DispatcherInstance->DispatchEvent(windowLostFocusing);
				}
				return DefWindowProc(hWnd, msg, wParam, lParam);
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
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_KEYDOWN:
			{
				GuardianKeyPressEvent keyPressing((UINT)wParam);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(keyPressing);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_KEYUP:
			{
				GuardianKeyReleaseEvent keyReleasing((UINT)wParam);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(keyReleasing);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_CHAR:
			{
				GuardianCharEvent charEvent((UCHAR)wParam);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(charEvent);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_MOUSEMOVE:
			{
				GuardianMouseMoveEvent mouseMoving((int)LOWORD(lParam), (int)HIWORD(lParam));

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseMoving);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_LBUTTONDOWN:
			{
				GuardianMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_LEFT);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_MBUTTONDOWN:
			{
				GuardianMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_MIDDLE);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_RBUTTONDOWN:
			{
				GuardianMouseButtonClickEvent mouseButtonClicking(GE_MOUSEBUTTON_RIGHT);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonClicking);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_LBUTTONUP:
			{
				GuardianMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_LEFT);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}
			case WM_MBUTTONUP:
			{
				GuardianMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_MIDDLE);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(hWnd, msg, wParam, lParam);
				break;
			}

			case WM_RBUTTONUP:
			{
				GuardianMouseButtonReleaseEvent mouseButtonReleasing(GE_MOUSEBUTTON_RIGHT);

				GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseButtonReleasing);
				return DefWindowProc(hWnd, msg, wParam, lParam);
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
							GuardianMouseRawMoveEvent mouseRawMoving((float)Raw->data.mouse.lLastX, (float)Raw->data.mouse.lLastY);

							GuardianEventDispatcher::DispatcherInstance->DispatchEvent(mouseRawMoving);
						}
					}
				}

				return DefWindowProc(hWnd, msg, wParam, lParam);
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
