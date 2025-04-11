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

		void InitializeMesh(std::shared_ptr<GuardianGraphics> graphics,
			const GString& meshName, std::vector<GuardianMeshInstance::Data> instanceData);
		void InitializeMesh(std::shared_ptr<GuardianGraphics> graphics,
			const GString& meshName, const GString& meshFilePath);
		void SetMeshName(const GString& meshName);
		void AddInstanceToMesh(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianMeshInstance::Data& instanceData);

		void UpdateMeshTransform(XMMATRIX worldMatrix);
		void UpdateMeshLighting(std::shared_ptr<GuardianGraphics> graphics,
			GuardianLightProperties properties);

		void SubmitToRenderer(const GString& submitFramebuffer);

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