#include "GCollider.h"

namespace GE
{
	GCollider::GCollider()
	{

	}

	GCollider::~GCollider()
	{
		for (auto& shape : this->ColliderShapeList)
		{
			shape.reset();
			shape = null;
		}
		this->ColliderShapeList.clear();
	}

	void GCollider::AddShape(std::shared_ptr<GShape> shape)
	{
		this->ColliderShapeList.push_back(shape);
	}

	void GCollider::InitializeCollider()
	{
		for (auto& shape : this->ColliderShapeList)
		{
			shape->InitializeShape();
		}
	}

	const std::vector<std::shared_ptr<GShape>>& GCollider::GetColliderShapeList() const noexcept
	{
		return this->ColliderShapeList;
	}
}
