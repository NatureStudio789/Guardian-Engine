#include "GuardianApplication.h"
#include "../GuardianEngine.h"

namespace GE
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
		GuardianWindowProperties StartupWindowProperties;
		StartupWindowProperties.SetWindowResolution(960, 540);
		StartupWindowProperties.SetWindowTitle("Guardian Engine - Initializing");
		StartupWindowProperties.SetWindowBorder(false);
		this->StartupWindow = GuardianWindow::CreateNewWindow(
			StartupWindowProperties, StartupWindowMessageProcessFunction);
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
				this->ApplicationWindow->WindowGraphicsContext->UpdateGUIGraphicsResolution(event.ResizeWidth, event.ResizeHeight);
			}
		});
	}

	void GuardianApplication::LaunchStartupWindow()
	{
		this->StartupWindow->DisplayWindow();

		while (!GuardianEngine::EngineInstance->FinishedInitialization)
		{
			this->StartupWindow->UpdateWindowMessage();
		}

		this->StartupWindow->DestroyWindow();

		GuardianThread::Sleep(1500);
	}

	void GuardianApplication::DisplayWindow()
	{
		this->ApplicationWindow->DisplayWindow();
	}

	void GuardianApplication::UpdateApplication() noexcept
	{
		this->ApplicationWindow->UpdateWindowMessage();
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
