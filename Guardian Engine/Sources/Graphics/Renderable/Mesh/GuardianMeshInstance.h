#ifndef _GE_GUARDIANMESHINSTANCE_H_
#define _GE_GUARDIANMESHINSTANCE_H_
#include "../GuardianRenderableBase.h"

namespace GE
{
	class GUARDIAN_API GuardianMeshInstance : public GuardianRenderableBase<GuardianMeshInstance>
	{
	public:
		struct Vertex
		{
			GVector3 Position;
			GVector2 TextureCoord;
			GVector3 Normal;
		};

		struct Data
		{
			GString MeshInstanceName;
			std::vector<Vertex> VertexData;
			std::vector<UINT> IndexData;
			GuardianUUID MaterialId;
		};

	public:
		GuardianMeshInstance();
		GuardianMeshInstance(const GuardianMeshInstance& other);
		~GuardianMeshInstance() override;

		void InitializeMeshInstance(const GuardianMeshInstance::Data& data);
		void SetMeshInstanceMaterial(const GuardianUUID& materialId);

		void Update() override;
		void UpdateMeshInstanceTransform(XMMATRIX worldMatrix);
		void UpdateMeshInstanceLighting(GuardianLightProperties properties);

		const Data& GetMeshInstanceData() const noexcept;
		const std::vector<GVector3> GetPositionVertices() const noexcept;
		const std::vector<UINT>& GetIndices() const noexcept;

		bool operator==(const GuardianMeshInstance& other) const;

	private:
		void CalculateBoundingBox();

		Data InstanceData;

		friend class GuardianMesh;
	};
}

#endif