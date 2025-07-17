#ifndef _GE_GPROGRAM_H_
#define _GE_GPROGRAM_H_
#include "../../Aplication/GApplication.h"

namespace GE
{
	class GUARDIAN_API GProgram
	{
	public:
		struct Attribute
		{
		public:
			std::string ProgramName;
			std::vector<std::string> ProgramRequiredModuleList;
			GWindow::Attribute ProgramWindowAttribute;
		};

	public:
		GProgram() {}
		virtual ~GProgram() { this->ReleaseProgram(); }

		virtual void PreInitializeProgram()
		{
			this->ProgramAttribute.ProgramName = "GuardianProgram";
			this->ProgramAttribute.ProgramRequiredModuleList = { "Render", "Physics", "Audio"};

			this->ProgramAttribute.ProgramWindowAttribute.WindowTitle = "Guardian Program Window";
			this->ProgramAttribute.ProgramWindowAttribute.WindowStyle = GWindow::GE_STYLE_DEFAULTWINDOW;
			this->ProgramAttribute.ProgramWindowAttribute.WindowTheme = GWindow::GE_THEME_DARK;
		}
		virtual void InitializeProgram() {}
		virtual void UpdateProgram() {}
		virtual void ReleaseProgram() {}

		virtual LRESULT CALLBACK ProcessWindowMessage(
			GWindow::Handle handle, UINT message, WPARAM wParam, LPARAM lParam) { return 0; }

		const Attribute& GetProgramAttribute() const noexcept
		{
			return this->ProgramAttribute;
		}
		const std::string& GetProgramName() const noexcept
		{
			return this->ProgramAttribute.ProgramName;
		}
		const std::vector<std::string> GetProgramRequiredModuleList() const noexcept
		{
			return this->ProgramAttribute.ProgramRequiredModuleList;
		}

	protected:
		Attribute ProgramAttribute;
	};
}

#endif