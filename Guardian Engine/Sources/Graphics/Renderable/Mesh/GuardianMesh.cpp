#include "GuardianMesh.h"

namespace guardian
{
	GuardianMesh::GuardianMesh()
	{
		
	}

	GuardianMesh::GuardianMesh(const GuardianMesh& other)
	{
		
	}

	GuardianMesh::~GuardianMesh()
	{
		for (auto& staticApplicableList : StaticApplicableList)
		{
			staticApplicableList.reset();
		}

		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
	}

	void GuardianMesh::Update()
	{
		
	}
}
