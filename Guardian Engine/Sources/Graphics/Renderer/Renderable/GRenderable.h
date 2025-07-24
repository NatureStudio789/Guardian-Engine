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
		GRenderable(const GRenderable& other);
		~GRenderable();

		void AddTechnology(std::shared_ptr<GTechnique> technology);
		void SetTechnologyList(std::vector<std::shared_ptr<GTechnique>> technologyList);

		void Apply();
		void Submit(const std::string& channel);

		void LinkTechnique(std::string renderGraphName);

		const GUUID& GetRenderableId() const noexcept;
		const std::string& GetRenderableName() const noexcept;
		const UINT& GetVerticesCount() const;
		const UINT& GetIndicesCount() const;
		const UINT& GetDataSize() const;
		const bool HasIndexBuffer() const noexcept;

	private:
		GUUID RenderableId = GUUID();
		std::string RenderableName;
		std::shared_ptr<GVertexBuffer> VertexBuffer;
		std::shared_ptr<GIndexBuffer> IndexBuffer;
		std::shared_ptr<GTopology> Topology;

		std::vector<std::shared_ptr<GTechnique>> RenderTechniqueList;
	};
}

#endif