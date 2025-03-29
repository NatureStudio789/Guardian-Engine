#include "GuardianFile.h"

namespace GE
{
	GuardianFile::GuardianFile()
	{
		this->FileExtention = "<Invalid>";
		this->FileName = "<Invalid>";
		this->FileSpecificPath = "<Invalid>";
		this->FileCategory = GE_INVALID_FILE;
	}

	GuardianFile::GuardianFile(const GString& filePath)
	{
		this->InitializeFile(filePath);
	}

	GuardianFile::GuardianFile(const GuardianFile& other)
	{
		this->FileSpecificPath = other.FileSpecificPath;
		this->FileName = other.FileName;
		this->FileCategory = other.FileCategory;
		this->FileExtention = other.FileExtention;
	}

	void GuardianFile::InitializeFile(const GString& filePath)
	{
		this->FileSpecificPath = filePath;
		if (this->IsFileExists())
		{
			GString NameWithExtention = this->FileSpecificPath;
			if (this->FileSpecificPath.find_last_of('/') != GString::npos)
			{
				size_t SlashPos = this->FileSpecificPath.find_last_of('/');
				
				NameWithExtention = this->FileSpecificPath.substr(SlashPos + 1);
			}
			else if (this->FileSpecificPath.find_last_of('\\') != GString::npos)
			{
				size_t SlashPos = this->FileSpecificPath.find_last_of('\\');

				NameWithExtention = this->FileSpecificPath.substr(SlashPos + 1);
			}

			this->FileName = NameWithExtention.substr(0, NameWithExtention.find_last_of('.'));

			this->FileExtention = NameWithExtention.substr(NameWithExtention.find_last_of('.'));
			if (this->FileExtention == ".txt")
			{
				this->FileCategory = GE_TEXT_FILE;
			}
			else if (this->FileExtention == ".png" ||
				this->FileExtention == ".jpg" ||
				this->FileExtention == ".bmp" ||
				this->FileExtention == ".jpeg" ||
				this->FileExtention == ".tga" ||
				this->FileExtention == ".ico")
			{
				this->FileCategory = GE_TEXTURE_FILE;
			}
			else if (this->FileExtention == ".obj" ||
				this->FileExtention == ".fbx" ||
				this->FileExtention == ".dae" ||
				this->FileExtention == ".stl" ||
				this->FileExtention == ".gltf" || 
				this->FileExtention == ".gmesh")
			{
				this->FileCategory = GE_MESH_FILE;
			}
			else if (this->FileExtention == ".gscene")
			{
				this->FileCategory = GE_SCENE_FILE;
			}
			else if (this->FileExtention == ".gproject")
			{
				this->FileCategory = GE_PROJECT_FILE;
			}
			else
			{
				this->FileCategory = GE_UNKNOWN_FILE;
			}
		}
		else
		{
			this->FileSpecificPath = "<Invalid>";
		}
	}

	const bool GuardianFile::IsFileExists() const noexcept
	{
		return std::filesystem::exists(this->FileName);
	}

	const GString& GuardianFile::GetFileSpecificPath() const noexcept
	{
		return this->FileSpecificPath;
	}

	const GString& GuardianFile::GetFileName() const noexcept
	{
		return this->FileName;
	}

	const GuardianFileCategory& GuardianFile::GetFileCategory() const noexcept
	{
		return this->FileCategory;
	}

	const GString& GuardianFile::GetFileExtention() const noexcept
	{
		return this->FileExtention;
	}
}
