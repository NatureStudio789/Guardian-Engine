#ifndef _GE_GUARDIANMESH_H_
#define _GE_GUARDIANMESH_H_
#include "../ModelImporter/GuardianModelImporter.h"

namespace GE
{
	class GUARDIAN_API GuardianMesh
	{
		using GuardianMeshInstanceId = GuardianUUID;
	public:
		GuardianMesh();
		GuardianMesh(const GuardianMesh& other);
		~GuardianMesh();

		void InitializeMesh(const GString& meshName, std::vector<GuardianMeshInstance::Data> instanceData);
		void InitializeMesh(const GString& meshName, const GString& meshFilePath);
		void SetMeshName(const GString& meshName);
		void AddInstanceToMesh(const GuardianMeshInstance::Data& instanceData);

		void UpdateMeshTransform(XMMATRIX worldMatrix);
		void UpdateMeshLighting(GuardianLightProperties properties);

		void SubmitToRenderer(const GString& submitRenderGraph);

		std::shared_ptr<GuardianMeshInstance> GetMeshInstance(const GuardianUUID& id);
		const GString& GetMeshName() const noexcept;

	private:
		GString MeshName;
		std::vector<std::shared_ptr<GuardianMeshInstance>> MeshInstancesList;

		friend class GuardianMaterialSystem;
		friend class GuardianSceneHierarchyPanel;
	};
}

#endif