#include "GApplication.h"

namespace GE
{
	std::shared_ptr<GApplication> GApplication::Instance = std::make_shared<GApplication>();


	GApplication::GApplication()
	{
		this->MainWindow = null;
	}

	GApplication::~GApplication()
	{
		this->MainWindow.reset();
		this->MainWindow = null;
	}

	void GApplication::InitializeApplication(const GWindow::Attribute& windowAttribute)
	{
		this->MainWindow = GWindow::CreateNewWindow(windowAttribute);
	}

	void GApplication::DisplayMainWindow()
	{
		this->MainWindow->DisplayWindow();
	}

	void GApplication::UpdateApplication()
	{
		this->MainWindow->UpdateWindow();
	}

	const bool& GApplication::IsApplicationRunning() const noexcept
	{
		return this->MainWindow->GetWindowRunning();
	}

	std::shared_ptr<GWindow> GApplication::GetMainWindow()
	{
		return this->MainWindow;
	}

	GWindow::Handle GApplication::GetMainWindowHandle()
	{
		GUARDIAN_CHECK_POINTER(this->MainWindow);
		return this->MainWindow->GetWindowHandle();
	}
}
