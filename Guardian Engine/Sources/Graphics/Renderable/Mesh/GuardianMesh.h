#ifndef _GE_GUARDIANMESH_H_
#define _GE_GUARDIANMESH_H_
#include "../GuardianRenderableBase.h"

namespace guardian
{
	class GUARDIAN_API GuardianMesh : public GuardianRenderableBase<GuardianMesh>
	{
	public:
		struct Vertex
		{
			GVector3 Position;
			GVector2 TextureCoord;
			GVector3 Normal;
		};

	public:
		GuardianMesh();
		GuardianMesh(const GuardianMesh& other);
		~GuardianMesh() override;

		void InitializeMesh(std::shared_ptr<GuardianGraphics> graphics,
			std::vector<Vertex> vertices, std::vector<UINT> indices);

		void Update() override;
		void UpdateMeshTransform(XMMATRIX transform);

		const std::vector<GVector3> GetPositionVertices() const noexcept;
		const std::vector<UINT>& GetIndices() const noexcept;
		std::shared_ptr<GuardianMaterial> GetMeshMaterial() noexcept;

	private:
		std::vector<Vertex> VertexData;
		std::vector<UINT> IndexData;
	};	
}

#endif