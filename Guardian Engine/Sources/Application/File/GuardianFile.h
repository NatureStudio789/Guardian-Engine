#ifndef _GE_GUARDIANFILE_H_
#define _GE_GUARDIANFILE_H_
#include "../Exception/GuardianException.h"

namespace GE
{
	enum GUARDIAN_API GuardianFileCategory
	{
		GE_INVALID_FILE = -1,
		GE_UNKNOWN_FILE = 0,
		GE_TEXT_FILE = 1,
		GE_TEXTURE_FILE = 2,
		GE_MESH_FILE = 3,
		GE_SCENE_FILE = 4,
		GE_PROJECT_FILE = 5,
	};

	class GUARDIAN_API GuardianFile
	{
	public:
		GuardianFile();
		GuardianFile(const GString& filePath);
		GuardianFile(const GuardianFile& other);

		void InitializeFile(const GString& filePath);

		const bool IsFileExists() const noexcept;

		const GString& GetFileSpecificPath() const noexcept;
		const GString& GetFileName() const noexcept;
		const GuardianFileCategory& GetFileCategory() const noexcept;
		const GString& GetFileExtention() const noexcept;

	private:
		GString FileSpecificPath;
		GString FileName;

		GuardianFileCategory FileCategory;
		GString FileExtention;
	};
}

#endif