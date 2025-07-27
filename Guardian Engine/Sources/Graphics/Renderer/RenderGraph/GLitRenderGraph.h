#ifndef _GE_GLITRENDERGRAPH_H_
#define _GE_GLITRENDERGRAPH_H_
#include "GRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GLitRenderGraph : public GRenderGraph
	{
	public:
		GLitRenderGraph(const std::string& name);

		static std::shared_ptr<GLitRenderGraph> CreateNewLitRenderGraph(const std::string& name)
		{
			return std::make_shared<GLitRenderGraph>(name);
		}

	private:

	};
}

#endif