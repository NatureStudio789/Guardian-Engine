#include "GShape.h"

namespace GE
{
	GShape::GShape()
	{
		this->LocalTransform = {};
		this->ShapeMaterial = std::make_shared<GPhysicsMaterial>();
		this->ShapeObject = null;
	}

	GShape::GShape(const GTransform& localTransform, std::shared_ptr<GPhysicsMaterial> material, PxShape* shape)
	{
		this->LocalTransform = localTransform;
		this->ShapeMaterial = material;
		this->ShapeObject = shape;
	}

	GShape::GShape(const GShape& other)
	{
		this->LocalTransform = other.LocalTransform;
		this->ShapeMaterial = other.ShapeMaterial;
		this->ShapeObject = other.ShapeObject;
	}

	GShape::~GShape()
	{
		this->LocalTransform = {};

		this->ShapeMaterial.reset();
		this->ShapeObject->release();
	}

	void GShape::SetLocalTransform(const GTransform& transform)
	{
		this->LocalTransform = transform;
	}

	void GShape::SetLocalPosition(const GVector3& position)
	{
		this->LocalTransform.Position = position;
	}

	void GShape::SetLocalRotation(const GVector3& rotation)
	{
		this->LocalTransform.Rotation = rotation;
	}

	void GShape::SetLocalScale(const GVector3& scale)
	{
		this->LocalTransform.Scale = scale;
	}

	const GTransform& GShape::GetLocalTransform() const noexcept
	{
		return this->LocalTransform;
	}

	std::shared_ptr<GPhysicsMaterial> GShape::GetShapeMaterial()
	{
		return this->ShapeMaterial;
	}

	PxShape* GShape::GetShapeObject()
	{
		return this->ShapeObject;
	}
}
