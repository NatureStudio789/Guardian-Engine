#ifndef _GE_GSHAPE_H_
#define _GE_GSHAPE_H_
#include "../../Material/GPhysicsMaterial.h"

namespace GE
{
	class GUARDIAN_API GShape
	{
	public:
		enum Category
		{
			GE_SHAPE_UNKNOWN = 0,
			GE_SHAPE_BOX,
			GE_SHAPE_SPHERE,
			GE_SHAPE_CAPSULE
		};

	public:
		GShape();
		GShape(const GTransform& localTransform, std::shared_ptr<GPhysicsMaterial> material, PxShape* shape);
		GShape(const GShape& other);
		virtual ~GShape();

		virtual void SetLocalTransform(const GTransform& transform);
		virtual void SetLocalPosition(const GVector3& position);
		virtual void SetLocalRotation(const GVector3& rotation);

		virtual void InitializeShape() {}

		virtual const Category GetShapeCategory() const noexcept
		{
			return GE_SHAPE_UNKNOWN;
		}
		virtual const GTransform& GetLocalTransform() const noexcept;

		virtual std::shared_ptr<GPhysicsMaterial> GetShapeMaterial();
		virtual PxShape* GetShapeObject();

	protected:
		GTransform LocalTransform;

		std::shared_ptr<GPhysicsMaterial> ShapeMaterial;
		PxShape* ShapeObject;

		friend class GSceneEditor;
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

		const Category GetShapeCategory() const noexcept override
		{
			return GE_SHAPE_BOX;
		}

		const GVector3& GetEdgeLength() const noexcept
		{
			return this->EdgeLength;
		}

		static std::shared_ptr<GBoxShape> CreateNewBoxShape(const GVector3& edgeLength)
		{
			return std::make_shared<GBoxShape>(edgeLength);
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

		const Category GetShapeCategory() const noexcept override
		{
			return GE_SHAPE_SPHERE;
		}

		float GetRadius() const noexcept
		{
			return this->Radius;
		}

		static std::shared_ptr<GSphereShape> CreateNewSphereShape(float radius)
		{
			return std::make_shared<GSphereShape>(radius);
		}

		float Radius;
	};

	class GUARDIAN_API GCapsuleShape : public GShape
	{
	public:
		GCapsuleShape()
		{
			this->HalfSphereRadius = 0.5f;
			this->Height = 2.0f;
		}
		GCapsuleShape(float halfSphereRadius, float height)
		{
			this->HalfSphereRadius = halfSphereRadius;
			this->Height = height;
		}
		GCapsuleShape(const GCapsuleShape& other) : GShape(other)
		{
			this->HalfSphereRadius = other.HalfSphereRadius;
			this->Height = other.Height;
		}

		void SetHalfSphereRadius(float halfSphereRadius)
		{
			this->HalfSphereRadius = halfSphereRadius;
		}
		void SetHeight(float height)
		{
			this->Height = height;
		}

		void InitializeShape() override
		{
			this->ShapeMaterial->InitializeMaterial();

			this->ShapeObject = GPhysicsContextRegistry::GetCurrentPhysicsContext()->GetPhysicsHandle()->createShape(
				PxCapsuleGeometry(this->HalfSphereRadius, this->Height / 2.0f),
				*this->ShapeMaterial->GetMaterialObject());

			if (!this->ShapeObject)
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to create capsule shape!");
			}

			auto& quaternion = GVector4::EulerToQuaternion(this->LocalTransform.Rotation);
			PxTransform transform(PxVec3(this->LocalTransform.Position.x, this->LocalTransform.Position.y, this->LocalTransform.Position.z),
				PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
			this->ShapeObject->setLocalPose(transform);
		}

		const Category GetShapeCategory() const noexcept override
		{
			return GE_SHAPE_CAPSULE;
		}

		float GetHalfSphereRadius() const noexcept
		{
			return this->HalfSphereRadius;
		}

		float GetHeight() const noexcept
		{
			return this->Height;
		}

		static std::shared_ptr<GCapsuleShape> CreateNewCapsuleShape(float halfSphereRadius, float height)
		{
			return std::make_shared<GCapsuleShape>(halfSphereRadius, height);
		}

		float Height;
		float HalfSphereRadius;
	};
}

#endif