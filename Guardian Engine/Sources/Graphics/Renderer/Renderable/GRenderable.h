#ifndef _GE_GRENDERABLE_H_
#define _GE_GRENDERABLE_H_
#include "../Technique/GTechnique.h"

namespace GE
{
	class GRenderGraph;

	class GUARDIAN_API GRenderable
	{
	public:
		GRenderable();
		GRenderable(const std::string& name, std::shared_ptr<GVertexBuffer> vertexBuffer, 
			std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology);
		GRenderable(const GRenderable& other);
		virtual ~GRenderable();

		void InitializeRenderable(const std::string& name, std::shared_ptr<GVertexBuffer> vertexBuffer,
			std::shared_ptr<GIndexBuffer> indexBuffer, std::shared_ptr<GTopology> topology);

		void AddTechnique(std::shared_ptr<GTechnique> technique);
		void SetTechniqueList(std::vector<std::shared_ptr<GTechnique>> techniqueList);

		void Apply();
		void Submit(const std::string& channel);
		void SetTransform(const GTransform& transform);

		void LinkTechnique(std::string renderGraphName);

		const GUUID& GetRenderableId() const noexcept;
		const std::string& GetRenderableName() const noexcept;
		const UINT& GetVerticesCount() const;
		const UINT& GetIndicesCount() const;
		const UINT& GetDataSize() const;
		const bool HasIndexBuffer() const noexcept;
		const GTransform& GetTransform() const noexcept;

	private:
		GUUID RenderableId = GUUID();
		std::string RenderableName;
		std::shared_ptr<GVertexBuffer> VertexBuffer;
		std::shared_ptr<GIndexBuffer> IndexBuffer;
		std::shared_ptr<GTopology> Topology;

		GTransform Transform;

		std::vector<std::shared_ptr<GTechnique>> RenderTechniqueList;
	};
}

#endif