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
		StaticApplicableList.clear();

		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();
	}

	void GuardianMesh::Update()
	{
		
	}
}
