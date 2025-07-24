#ifndef _GE_GRENDERGRAPH_H_
#define _GE_GRENDERGRAPH_H_
#include "../Pass/GRenderQueuePass.h"

namespace GE
{
	class GUARDIAN_API GRenderGraph
	{
	public:
		GRenderGraph();
		GRenderGraph(const std::string& name);
		GRenderGraph(const GRenderGraph& other);
		~GRenderGraph();

		void InitializeRenderGraph(const std::string& name);

		virtual void Execute();
		virtual void Reset();

		const GUUID& GetRenderGraphId() const noexcept;
		const std::string& GetRenderGraphName() const noexcept;

		std::shared_ptr<GPass> GetPass(const std::string& passName);

		static std::shared_ptr<GRenderGraph> CreateNewRenderGraph(const std::string& name)
		{
			return std::make_shared<GRenderGraph>(name);
		}

	protected:
		// Must call this function before do any graphics modification.
		void InitializeGraphGraphics();

		void SetSinkTarget(const std::string& sinkName, const std::string& target);
		void AddGlobalSource(std::shared_ptr<GSource> source);
		void AddGlobalSink(std::shared_ptr<GSink> sink);
		void Finalize();
		void AppendPass(std::shared_ptr<GPass> pass);

		std::shared_ptr<GFramebuffer> RenderGraphFramebuffer;

	private:
		void LinkSinks(std::shared_ptr<GPass> pass);
		void LinkGlobalSinks();

		std::string RenderGraphName;
		GUUID RenderGraphId;

		bool IsFinalized = false;
		std::vector<std::shared_ptr<GSource>> GlobalSourceList;
		std::vector<std::shared_ptr<GSink>> GlobalSinkList;

		std::map<std::string, std::shared_ptr<GPass>> PassList;
	};
}

#endif