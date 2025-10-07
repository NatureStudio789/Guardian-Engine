#ifndef _GE_GEDITORENGINE_H_
#define _GE_GEDITORENGINE_H_
#include "EditorRenderGraph/GEditorRenderGraph.h"
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

		LRESULT CALLBACK ProcessWindowMessage(
			GWindow::Handle handle, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		std::shared_ptr<GAssetLoader> EditorAssetLoader;

		std::shared_ptr<GEditorRenderGraph> EditorRenderGraph;
		std::shared_ptr<GMission> EditorRenderMission;
	};
}

GUARDIAN_SUBMIT_PROGRAM(GE::GEditorEngine)

#endif