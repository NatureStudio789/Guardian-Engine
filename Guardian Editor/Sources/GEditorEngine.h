#ifndef _GE_GEDITORENGINE_H_
#define _GE_GEDITORENGINE_H_
#include <Profile/EntryPoint/GEntryPoint.h>

namespace GE
{
	class GEditorEngine : public GProgram
	{
	public:
		GEditorEngine();
		~GEditorEngine() override;

		void PreInitializeProgram() override;
		void InitializeProgram() override;
		void UpdateProgram() override;
		void ReleaseProgram() override;

	private:

	};
}

GUARDIAN_SUBMIT_PROGRAM(GE::GEditorEngine)

#endif