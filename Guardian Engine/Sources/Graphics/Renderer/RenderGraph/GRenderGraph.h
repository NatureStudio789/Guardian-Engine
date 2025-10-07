#ifndef _GE_GRENDERGRAPH_H_
#define _GE_GRENDERGRAPH_H_
#include "../Pass/GLightingPass.h"

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

		/*This function must be and only can be called before calling the Execute() function.
		Or the framebuffer for render graph would be invalid or wouldn't be updated!*/
		virtual void SetFramebuffer(std::shared_ptr<GFramebuffer> framebuffer);
		/*This function can be called per frame update. 
		It will be updated to each pass which has sink or source of camera.*/
		virtual void SetCamera(std::shared_ptr<GCamera> camera);

		virtual void Execute();
		virtual void Reset();
		virtual void Resize(int newWidth, int newHeight);
		/*For linking the sink-source, must call it before calling Execute() function.*/
		void Finalize();

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