#ifndef _GE_GCOLLIDER_H_
#define _GE_GCOLLIDER_H_
#include "Shape/GShape.h"

namespace GE
{
	class GUARDIAN_API GCollider
	{
	public:
		GCollider();
		~GCollider();

		void AddShape(std::shared_ptr<GShape> shape);
		void InitializeCollider();

		const std::vector<std::shared_ptr<GShape>>& GetColliderShapeList() const noexcept;

	private:
		std::vector<std::shared_ptr<GShape>> ColliderShapeList;

		friend class GDynamicRigidBody;
		friend class GStaticRigidBody;
	};
}

#endif