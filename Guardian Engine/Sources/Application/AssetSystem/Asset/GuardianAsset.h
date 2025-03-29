#ifndef _GE_GUARDIANASSET_H_
#define _GE_GUARDIANASSET_H_
#include "../../../Profile/Program/GuardianProgram.h"

namespace GE
{
	class GUARDIAN_API GuardianAsset
	{
	public:
		GuardianAsset();
		GuardianAsset(const GuardianAsset& other);

		virtual const bool IsAvailable() const noexcept;

		const GString& GetAssetPath() const noexcept;
		const GString& GetAssetName() const noexcept;

	protected:
		void SetAssetPath(const GString& filePath);

		GString AssetName;
		GString AssetPath;
	};
}

#endif