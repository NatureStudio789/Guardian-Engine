#include "GuardianMeshCollider.h"

namespace GE
{
	GuardianMeshCollider::GuardianMeshCollider()
	{
		this->ColliderMeshShape = null;
		this->ColliderProperties = GuardianMeshColliderProperties();
		this->ColliderMaterial = GuardianPhysicsMaterial();
	}

	GuardianMeshCollider::GuardianMeshCollider(const GuardianMeshCollider& other)
	{
		this->ColliderMeshShape = other.ColliderMeshShape;
		this->ColliderProperties = other.ColliderProperties;
		this->ColliderMaterial = other.ColliderMaterial;
	}

	GuardianMeshCollider::~GuardianMeshCollider()
	{
	}

	void GuardianMeshCollider::SetColliderProperties(const GuardianMeshColliderProperties& properties)
	{
		this->ColliderProperties = properties;
	}

	void GuardianMeshCollider::SetColliderMaterial(const GuardianPhysicsMaterial& material)
	{
		this->ColliderMaterial = material;
		if (!this->ColliderMaterial.GetMaterialObject())
		{
			this->ColliderMaterial.InitializePhysicsMaterial();
		}
	}

	void GuardianMeshCollider::InitializeMeshCollider()
	{
		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = (PxU32)this->ColliderProperties.Vertices.size();
		meshDesc.points.stride = sizeof(GVector3);
		meshDesc.points.data = (const void*)this->ColliderProperties.Vertices.data();

		meshDesc.triangles.count = (PxU32)(this->ColliderProperties.Indices.size() / 3);
		meshDesc.triangles.stride = (PxU32)(3 * sizeof(PxU32));
		meshDesc.triangles.data = (const void*)this->ColliderProperties.Indices.data();

		PxTolerancesScale scale;
		PxCookingParams params(scale);

		PxDefaultMemoryOutputStream writeBuffer;
		if (!PxCookTriangleMesh(params, meshDesc, writeBuffer))
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to cook triangle mesh!");
		}

		PxDefaultMemoryInputData ReadBuffer(writeBuffer.getData(), writeBuffer.getSize());
		PxTriangleMesh* mesh = GuardianPhysicsEngine::GetPhysicsObject()->createTriangleMesh(ReadBuffer);

		this->ColliderMeshShape = GuardianPhysicsEngine::GetPhysicsObject()->createShape(PxTriangleMeshGeometry(mesh),
			*this->ColliderMaterial.GetMaterialObject(), false);
	}

	PxShape* GuardianMeshCollider::GetColliderShape() noexcept
	{
		return this->ColliderMeshShape;
	}

	const GuardianMeshColliderProperties& GuardianMeshCollider::GetColliderProperties() const noexcept
	{
		return this->ColliderProperties;
	}

	const GuardianPhysicsMaterial& GuardianMeshCollider::GetColliderMaterial() const noexcept
	{
		return this->ColliderMaterial;
	}
}
