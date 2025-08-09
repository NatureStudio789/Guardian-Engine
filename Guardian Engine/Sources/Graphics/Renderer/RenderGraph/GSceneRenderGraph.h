#ifndef _GE_GSCENERENDERGRAPH_H_
#define _GE_GSCENERENDERGRAPH_H_
#include "GLitRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GSceneRenderGraph : public GRenderGraph
	{
	public:
		GSceneRenderGraph(const std::string& name);

		void Execute() override;

		static std::shared_ptr<GSceneRenderGraph> CreateNewSceneRenderGraph(const std::string& name)
		{
			return std::make_shared<GSceneRenderGraph>(name);
		}

	private:
		void SetRuntimeCamera(std::shared_ptr<GCamera> runtimeCamera);

		std::shared_ptr<GCamera> RuntimeCamera;
		std::shared_ptr<GFramebuffer> RuntimeFramebuffer;
	};
}

#endif