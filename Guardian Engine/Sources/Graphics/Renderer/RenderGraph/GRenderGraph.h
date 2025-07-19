#ifndef _GE_GRENDERGRAPH_H_
#define _GE_GRENDERGRAPH_H_
#include "../Pass/GPass.h"

namespace GE
{
	class GUARDIAN_API GRenderGraph
	{
	public:
		GRenderGraph();
		GRenderGraph(const GRenderGraph& other);
		~GRenderGraph();

		virtual void Execute();
		virtual void Release();

	protected:
		void AppendPass(std::shared_ptr<GPass> pass);

	private:
		std::map<std::string, std::shared_ptr<GPass>> PassList;
	};
}

#endif