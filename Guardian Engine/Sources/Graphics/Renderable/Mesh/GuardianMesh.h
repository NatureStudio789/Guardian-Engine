#ifndef _GE_GUARDIANMESH_H_
#define _GE_GUARDIANMESH_H_
#include "../GuardianRenderableBase.h"

namespace guardian
{
	class GUARDIAN_API GuardianMesh : public GuardianRenderableBase<GuardianMesh>
	{
	public:
		GuardianMesh();
		GuardianMesh(const GuardianMesh& other);
		~GuardianMesh() override;

		void Update() override;
	};	
}

#endif