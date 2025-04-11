#ifndef _GE_GUARDIANFILE_H_
#define _GE_GUARDIANFILE_H_
#include "../UUID/GuardianUUID.h"

namespace GE
{
	class GUARDIAN_API GuardianFile
	{
	public:
		enum Category
		{
			GE_INVALID_FILE = -1,
			GE_UNKNOWN_FILE = 0,
			GE_TEXT_FILE = 1,
			GE_TEXTURE_FILE = 2,
			GE_MESH_FILE = 3,
			GE_SCENE_FILE = 4,
			GE_PROJECT_FILE = 5,
		};

	public:
		GuardianFile();
		GuardianFile(const GString& filePath);
		GuardianFile(const GuardianFile& other);

		void InitializeFile(const GString& filePath);

		void ClearFile();

		const bool IsFileExists() const noexcept;

		const GString& GetFileSpecificPath() const noexcept;
		const GString& GetFileName() const noexcept;
		const Category& GetFileCategory() const noexcept;
		const GString& GetFileExtention() const noexcept;

	protected:
		GString FileSpecificPath;
		GString FileName;

		Category FileCategory;
		GString FileExtention;
	};
}

#endif