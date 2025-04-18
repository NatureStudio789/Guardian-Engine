#ifndef _GE_GUARDIANRENDERGRAPH_H_
#define _GE_GUARDIANRENDERGRAPH_H_
#include "../Renderable/Geometry/GuardianGeometry.h"

namespace GE
{
	class GUARDIAN_API GuardianRenderGraph
	{
	public:
		GuardianRenderGraph();
		GuardianRenderGraph(std::shared_ptr<GuardianGraphics> graphGraphics, 
			const GString& graphName, int width, int height);
		GuardianRenderGraph(const GuardianRenderGraph& other);
		virtual ~GuardianRenderGraph();

		void InitializeRenderGraph(std::shared_ptr<GuardianGraphics> graphGraphics, 
			const GString& graphName, int width, int height);
		void SetGraphClearColor(const GVector3& color);
		void SetGraphCamera(const GuardianCamera& camera);
		void SetGraphVertexShader(const GString& shaderPath, D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutSize);
		void SetGraphPixelShader(const GString& shaderPath);

		void SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable);

		void Render();

		void ResizeRenderGraph(int width, int height);

		std::shared_ptr<GuardianFramebuffer> GetRenderGraphFramebuffer();

	protected:
		GString GraphName;

		GVector3 GraphClearColor;
		std::shared_ptr<GuardianGraphics> GraphGraphics;

		std::shared_ptr<GuardianVertexShader> GraphVertexShader;
		std::shared_ptr<GuardianPixelShader> GraphPixelShader;
		std::shared_ptr<GuardianInputLayout> GraphInputLayout;
		
		std::shared_ptr<GuardianFramebuffer> GraphFramebuffer;
		GuardianCamera GraphCamera;
		std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>> RenderableQueueList;
	
	private:

		friend class GuardianRenderer;
	};
}

#endif