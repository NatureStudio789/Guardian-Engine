#ifndef _GE_GRENDERGRAPH_H_
#define _GE_GRENDERGRAPH_H_
#include "../Pass/GLightingPass.h"

namespace GE
{
	class GUARDIAN_API GRenderGraph
	{
	public:
		GRenderGraph();
		GRenderGraph(const std::string& name, bool enableRTT = false);
		GRenderGraph(const GRenderGraph& other);
		~GRenderGraph();

		void InitializeRenderGraph(const std::string& name, bool enableRTT = false);

		void SetCamera(std::shared_ptr<GCamera> camera);

		virtual void Execute();
		virtual void Reset();
		virtual void Resize(int newWidth, int newHeight);

		const GUUID& GetRenderGraphId() const noexcept;
		const std::string& GetRenderGraphName() const noexcept;
		std::shared_ptr<GCamera> GetRenderGraphCamera();
		std::shared_ptr<GFramebuffer> GetRenderGraphFramebuffer();

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
		std::shared_ptr<GCamera> RenderGraphCamera;

		std::string RenderGraphName;
		GUUID RenderGraphId;

		std::map<std::string, std::shared_ptr<GPass>> PassList;
	
	private:
		void LinkSinks(std::shared_ptr<GPass> pass);
		void LinkGlobalSinks();

		bool IsFinalized = false;
		std::vector<std::shared_ptr<GSource>> GlobalSourceList;
		std::vector<std::shared_ptr<GSink>> GlobalSinkList;
	};
}

#endif