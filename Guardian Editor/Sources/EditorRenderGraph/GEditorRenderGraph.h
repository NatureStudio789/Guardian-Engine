#ifndef _GE_GEDITORRENDERGRAPH_H_
#define _GE_GEDITORRENDERGRAPH_H_
#include "../EditorContext/GEditorContext.h"

namespace GE
{
	class GEditorRenderGraph : public GRenderGraph
	{
	public:
		GEditorRenderGraph(const std::string& name);

		void Execute() override;
		void Reset() override;

	private:
		std::shared_ptr<GEditorContext> EditorContext;
		std::shared_ptr<GEventProcesser> EditorEventProcesser;
	};
}

#endif