#include "GuardianApplication.h"
#include "../GuardianEngine.h"

namespace GE
{
	std::unique_ptr<GuardianApplication> GuardianApplication::ApplicationInstance = std::make_unique<GuardianApplication>();
	GuardianApplication::CommandLineArgument GuardianApplication::ApplicationCommandLineArgument;


	GuardianApplication::GuardianApplication()
	{
		this->StartupWindow = std::make_shared<GuardianWindow>();
		this->CreatorWindow = std::make_shared<GuardianWindow>();
		this->MainWindow = std::make_shared<GuardianWindow>();
		this->ApplicationEventProcesser = std::make_unique<GuardianEventProcesser>();
	}

	GuardianApplication::~GuardianApplication()
	{

	}

	void GuardianApplication::InitializeApplication(const GuardianWindowProperties& windowProperties)
	{
		GuardianWindowProperties StartupWindowProperties;
		StartupWindowProperties.SetWindowResolution(960, 540);
		StartupWindowProperties.SetWindowTitle("Guardian Engine - Initializing");
		StartupWindowProperties.SetWindowBorder(false);
		this->StartupWindow = GuardianWindow::CreateNewWindow(
			StartupWindowProperties, StartupWindowMessageProcessFunction);

		GuardianWindowProperties CreatorWindowProperties;
		CreatorWindowProperties.SetWindowResolution(960, 540);
		CreatorWindowProperties.SetWindowTitle("Guardian Engine - Creator");
		this->CreatorWindow = GuardianWindow::CreateNewWindow(
			CreatorWindowProperties, CreatorWindowMessageProcessFunction);

		this->MainWindow = GuardianWindow::CreateNewWindow(
			windowProperties, GuardianApplication::MainWindowMessageProcessFunction);
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to create engine window.");

		GuardianInput::InitializeInput();
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to initialize input system.");

		this->ApplicationEventProcesser->OnEvent<GuardianWindowCloseEvent>([this](const GuardianWindowCloseEvent& event)
		{
			if (this->MainWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->MainWindow->IsWindowRunning = false;
			}
			else if (this->CreatorWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->CreatorWindow->IsWindowRunning = false;
			}
		});

		this->ApplicationEventProcesser->OnEvent<GuardianWindowResizeEvent>([this](const GuardianWindowResizeEvent& event)
		{
			if (this->MainWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->MainWindow->WindowGraphicsContext->UpdateGUIGraphicsResolution(event.ResizeWidth, event.ResizeHeight);
			}
			else if (this->CreatorWindow->GetWindowHandle() == event.WindowHandle)
			{
				this->CreatorWindow->WindowGraphicsContext->UpdateGUIGraphicsResolution(event.ResizeWidth, event.ResizeHeight);
			}
		});

		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to initialize engine application.");
	}

	void GuardianApplication::LaunchStartupWindow()
	{
		this->StartupWindow->DisplayWindow();
		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Displayed startup window.");

		while (!GuardianEngine::EngineInstance->FinishedInitialization)
		{
			this->StartupWindow->UpdateWindowMessage();
		}

		this->StartupWindow->DestroyWindow();

		GuardianThread::Sleep(1500);
	}

	void GuardianApplication::DisplayCreatorWindow()
	{
		this->CreatorWindow->DisplayWindow();
	}

	void GuardianApplication::DisplayMainWindow()
	{
		this->MainWindow->DisplayWindow();
	}

	void GuardianApplication::UpdateApplication() noexcept
	{
		this->MainWindow->UpdateWindowMessage();
	}

	void GuardianApplication::EndUpRendering(int syncInterval)
	{
		this->MainWindow->WindowGraphicsContext->EndUpRendering(syncInterval);
	}

	const bool GuardianApplication::IsApplicationRunning() const noexcept
	{
		return this->MainWindow->GetIsWindowRunning();
	}

	std::shared_ptr<GuardianWindow> GuardianApplication::GetCreatorWindow() noexcept
	{
		return this->CreatorWindow;
	}

	GWindowHandle GuardianApplication::GetCreatorWindowHandle() noexcept
	{
		return this->CreatorWindow->GetWindowHandle();
	}

	std::shared_ptr<GuardianWindow> GuardianApplication::GetMainWindow() noexcept
	{
		return this->MainWindow;
	}

	GWindowHandle GuardianApplication::GetMainWindowHandle() noexcept
	{
		return this->MainWindow->WindowHandle;
	}

	std::shared_ptr<GuardianGraphics> GuardianApplication::GetCreatorGraphicsContext() noexcept
	{
		return this->CreatorWindow->GetWindowGraphicsContext();
	}

	std::shared_ptr<GuardianGraphics> GuardianApplication::GetMainGraphicsContext() noexcept
	{
		return this->MainWindow->GetWindowGraphicsContext();
	}

	void GuardianApplication::SetCommandLineArgument(int argc, char* argv[])
	{
		for (int i = 0; i < argc; i++)
		{
			ApplicationCommandLineArgument.ArgumentList.push_back(argv[i]);
		}
	}

	LRESULT GuardianApplication::StartupWindowMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		static BOOL isDragging = FALSE;
		static int startX, startY;

		switch (msg)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);

			return 0;
			break;
		}

		case WM_SYSCOMMAND:
		{
			return 0;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			isDragging = TRUE;
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
			
			return 0;
			break;
		}

		case WM_MOUSEMOVE:
		{
			if (isDragging)
			{
				int currentX = LOWORD(lParam);
				int currentY = HIWORD(lParam);
				int offsetX = currentX - startX;
				int offsetY = currentY - startY;

				RECT newRect;
				GetWindowRect(hWnd, &newRect);
				int width = newRect.right - newRect.left;
				int height = newRect.bottom - newRect.top;
				SetWindowPos(hWnd, null, newRect.left + offsetX, newRect.top + offsetY, 960, 540, 0);
			}

			return 0;
			break;
		}

		case WM_LBUTTONUP:
		{
			isDragging = FALSE;
			
			return 0;
			break;
		}

		case WM_PAINT:
		{
			HDC hdc, LocalDC;
			PAINTSTRUCT ps;

			hdc = BeginPaint(hWnd, &ps);

			LocalDC = CreateCompatibleDC(hdc);
			HBITMAP Bitmap = (HBITMAP)LoadImage(null, L"..\\Guardian Engine\\Resources\\Startup.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (Bitmap == null)
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to load startup image!");
			}
			BITMAP qBitmap;
			GetObject((HGDIOBJ)Bitmap, sizeof(BITMAP), (LPVOID)&qBitmap);
			SelectObject(LocalDC, Bitmap);

			if (!BitBlt(hdc, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight, LocalDC, 0, 0, SRCCOPY))
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to bit blit!");
			}

			DeleteDC(LocalDC);
			DeleteObject(Bitmap);

			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));

			SetTextAlign(hdc, TA_CENTER);

			GString message = "Guardian Engine - Initializing";
			TextOutA(hdc, 960 / 2, 540 - 30, message.c_str(), (int)message.size());

			EndPaint(hWnd, &ps);

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

	LRESULT GuardianApplication::CreatorWindowMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (GuardianEngine::EngineInstance->EngineCreator->EditorMessageProcess(hWnd, msg, wParam, lParam))
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

			case WM_GETMINMAXINFO:
			{
				MINMAXINFO* MinMaxInfo = (MINMAXINFO*)lParam;

				MinMaxInfo->ptMinTrackSize.x = 1000;
				MinMaxInfo->ptMinTrackSize.y = 600;

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

	LRESULT GuardianApplication::MainWindowMessageProcessFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

			case WM_GETMINMAXINFO:
			{
				MINMAXINFO* MinMaxInfo = (MINMAXINFO*)lParam;

				MinMaxInfo->ptMinTrackSize.x = 1000;
				MinMaxInfo->ptMinTrackSize.y = 600;

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
