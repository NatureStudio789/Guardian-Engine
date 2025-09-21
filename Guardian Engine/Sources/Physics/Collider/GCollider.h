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

		void AddShape(std::shared_ptr<GBoxShape> shape);
		void AddShape(std::shared_ptr<GSphereShape> shape);
		void InitializeCollider();

	private:
		std::vector<std::shared_ptr<GBoxShape>> BoxShapeList;
		std::vector<std::shared_ptr<GSphereShape>> SphereShapeList;
	};
}

#endif