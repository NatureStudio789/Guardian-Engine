#ifndef _GE_GUARDIANMESHCOLLIDER_H_
#define _GE_GUARDIANMESHCOLLIDER_H_
#include "GuardianCapsuleCollider.h"

namespace GE
{
	struct GUARDIAN_API GuardianMeshColliderProperties
	{
		GuardianMeshColliderProperties()
		{
			this->Vertices.clear();
			this->Indices.clear();
		}
		GuardianMeshColliderProperties(const std::vector<GVector3>& vertices, const std::vector<UINT>& indices)
		{
			this->Vertices = vertices;
			this->Indices = indices;
		}

		std::vector<GVector3> Vertices;
		std::vector<UINT> Indices;
	};
	
	class GUARDIAN_API GuardianMeshCollider : public GuardianCollider
	{
	public:
		GuardianMeshCollider();
		GuardianMeshCollider(const GuardianMeshCollider& other);
		~GuardianMeshCollider();

		void SetColliderProperties(const GuardianMeshColliderProperties& properties);
		void SetColliderMaterial(const GuardianPhysicsMaterial& material);
		void InitializeMeshCollider();

		PxShape* GetColliderShape() noexcept override;
		const GuardianMeshColliderProperties& GetColliderProperties() const noexcept;
		const GuardianPhysicsMaterial& GetColliderMaterial() const noexcept;

	private:
		PxShape* ColliderMeshShape;

		GuardianMeshColliderProperties ColliderProperties;
		GuardianPhysicsMaterial ColliderMaterial;
	};
}

#endif