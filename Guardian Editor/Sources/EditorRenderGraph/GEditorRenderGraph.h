#ifndef _GE_GEDITORRENDERGRAPH_H_
#define _GE_GEDITORRENDERGRAPH_H_
#include "../Editor/SceneEditor/GSceneEditor.h"
#include "../Editor/AssetEditor/GAssetEditor.h"

namespace GE
{
	class GEditorRenderGraph : public GRenderGraph
	{
	public:
		GEditorRenderGraph(const std::string& name);

		void Execute() override;
		void Reset() override;

	private:
		void AddEditor(std::shared_ptr<EUI::GEditor> editor);

		std::shared_ptr<GEditorContext> EditorContext;
		std::shared_ptr<GEventProcesser> EditorEventProcesser;

		std::shared_ptr<EUI::GDockspace> EditorMainDockspace;
	};
}

#endif