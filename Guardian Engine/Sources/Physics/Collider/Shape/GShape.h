#ifndef _GE_GSHAPE_H_
#define _GE_GSHAPE_H_
#include "../../Material/GPhysicsMaterial.h"

namespace GE
{
	class GUARDIAN_API GShape
	{
	public:
		GShape();
		GShape(const GTransform& localTransform, std::shared_ptr<GPhysicsMaterial> material, PxShape* shape);
		GShape(const GShape& other);
		virtual ~GShape();

		virtual void SetLocalTransform(const GTransform& transform);
		virtual void SetLocalPosition(const GVector3& position);
		virtual void SetLocalRotation(const GVector3& rotation);
		virtual void SetLocalScale(const GVector3& scale);

		virtual void InitializeShape() {}

		virtual const GTransform& GetLocalTransform() const noexcept;

		virtual std::shared_ptr<GPhysicsMaterial> GetShapeMaterial();
		virtual PxShape* GetShapeObject();

	protected:
		GTransform LocalTransform;

		std::shared_ptr<GPhysicsMaterial> ShapeMaterial;
		PxShape* ShapeObject;
	};

	class GUARDIAN_API GBoxShape : public GShape
	{
	public:
		GBoxShape() : GShape()
		{
			this->EdgeLength = { 1.0f };
		}
		GBoxShape(const GVector3& edgeLength)
		{
			this->SetEdgeLength(edgeLength);
		}
		GBoxShape(const GBoxShape& other) : GShape(other)
		{
			this->EdgeLength = other.EdgeLength;
		}

		void SetEdgeLength(const GVector3& edgeLength)
		{
			this->EdgeLength = edgeLength;
		}

		void InitializeShape() override
		{
			this->ShapeMaterial->InitializeMaterial();

			this->ShapeObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createShape(
				PxBoxGeometry(PxVec3(this->EdgeLength.x / 2.0f, this->EdgeLength.y / 2.0f, this->EdgeLength.z / 2.0f)),
				*this->ShapeMaterial->GetMaterialObject());

			if (!this->ShapeObject)
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to create box shape!");
			}

			auto& quaternion = GVector4::EulerToQuaternion(this->LocalTransform.Rotation);
			PxTransform transform(PxVec3(this->LocalTransform.Position.x, this->LocalTransform.Position.y, this->LocalTransform.Position.z),
				PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
			this->ShapeObject->setLocalPose(transform);
		}

		const GVector3& GetEdgeLength() const noexcept
		{
			return this->EdgeLength;
		}

		GVector3 EdgeLength;
	};

	class GUARDIAN_API GSphereShape : public GShape
	{
	public:
		GSphereShape()
		{
			this->Radius = 0.5f;
		}
		GSphereShape(float radius)
		{
			this->Radius = radius;
		}
		GSphereShape(const GSphereShape& other) : GShape(other)
		{
			this->Radius = other.Radius;
		}

		void SetRadius(float radius)
		{
			this->Radius = radius;
		}

		void InitializeShape() override
		{
			this->ShapeMaterial->InitializeMaterial();

			this->ShapeObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createShape(
				PxSphereGeometry(this->Radius),
				*this->ShapeMaterial->GetMaterialObject());

			if (!this->ShapeObject)
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to create sphere shape!");
			}

			auto& quaternion = GVector4::EulerToQuaternion(this->LocalTransform.Rotation);
			PxTransform transform(PxVec3(this->LocalTransform.Position.x, this->LocalTransform.Position.y, this->LocalTransform.Position.z),
				PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
			this->ShapeObject->setLocalPose(transform);
		}

		float Radius;
	};
}

#endif