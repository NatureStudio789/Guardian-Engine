#ifndef _GE_GAPPLICATION_H_
#define _GE_GAPPLICATION_H_
#include "../Profile/Module/GModuleSystem.h"

namespace GE
{
	class GUARDIAN_API GApplication
	{
	public:
		GApplication();
		GApplication(const GApplication&) = delete;
		~GApplication();

		void InitializeApplication(const GWindow::Attribute& windowAttribute);

		void DisplayMainWindow();
		void UpdateApplication();

		const bool& IsApplicationRunning() const noexcept;

		std::shared_ptr<GWindow> GetMainWindow();
		GWindow::Handle GetMainWindowHandle();

		static std::shared_ptr<GApplication> Instance;

	private:
		std::shared_ptr<GWindow> MainWindow;
	};
}

#endif