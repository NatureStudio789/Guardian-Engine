#ifndef _GE_GUARDIANMESHASSET_H_
#define _GE_GUARDIANMESHASSET_H_
#include "GuardianAsset.h"

namespace guardian
{
	struct GUARDIAN_API GuardianMeshInstance::Data;

	class GUARDIAN_API GuardianMeshAsset : public GuardianAsset
	{
	public:
		GuardianMeshAsset();
		GuardianMeshAsset(const GString& filePath);
		GuardianMeshAsset(const GuardianMeshAsset& other);

		void InitializeMeshAsset(const GString& filePath);

		const bool IsAvailable() const noexcept	override;

		const std::vector<GuardianMeshInstance::Data>& GetMeshAssetData() const noexcept;

	private:
		std::vector<GuardianMeshInstance::Data>	MeshAssetData;
	};
}

#endif