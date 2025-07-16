#ifndef _GE_GRENDERABLE_H_
#define _GE_GRENDERABLE_H_
#include "../Applicable/PipelineState/GPipelineState.h"

namespace GE
{
	class GUARDIAN_API GRenderable
	{
	public:
		GRenderable();
		GRenderable(const GRenderable& other);
		virtual ~GRenderable();

		void AddApplicable(std::shared_ptr<GApplicable> applicable);
		void AddVertexBuffer(std::shared_ptr<GVertexBuffer> vertexBuffer);
		void AddIndexBuffer(std::shared_ptr<GIndexBuffer> indexBuffer);
		void AddTransformCBuffer(std::shared_ptr<GTransformCBuffer> cbuffer);

		void Render();
		virtual void Update() {}
		void UpdateTransformData(GTransformCBData data);

		const GUUID& GetRenderableId() const noexcept;
		std::shared_ptr<GVertexBuffer> GetVertexBuffer();
		std::shared_ptr<GIndexBuffer> GetIndexBuffer();

	protected:
		GUUID RenderableId;
		std::shared_ptr<GVertexBuffer> VertexBuffer;
		std::shared_ptr<GIndexBuffer> IndexBuffer;
		std::shared_ptr<GTransformCBuffer> TransformCBuffer;

		std::vector<std::shared_ptr<GApplicable>> ApplicableList;

		virtual std::vector<std::shared_ptr<GApplicable>> GetStaticApplicableList() noexcept
		{
			return std::vector<std::shared_ptr<GApplicable>>();
		}
		virtual std::shared_ptr<GPipelineState> GetRenderablePipelineState() noexcept
		{
			return null;
		}
	};
}

#endif