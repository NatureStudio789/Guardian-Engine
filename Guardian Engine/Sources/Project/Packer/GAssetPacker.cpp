#include "GAssetPacker.h"
#include "../../Gameplay/Serializer/GAssetSerializer.h"
#include "../../Asset/Loader/GAssetLoader.h"

namespace GE
{
	void GAssetPacker::PackAsset(std::shared_ptr<GAssetLoader> assetLoader, const std::string& packageFilePath)
	{
		GUARDIAN_CHECK_POINTER(assetLoader);

		std::vector<std::string> AssetAndSourceFileList;
		for (const auto& asset : assetLoader->GetLoadedAssetList())
		{
			AssetAndSourceFileList.push_back(asset.second->GetAssetPath());
			AssetAndSourceFileList.push_back(asset.second->GetAssetSourcePath());
		}

		std::vector<GAssetPackEntry> EntryList;
		unsigned long long CurrentDataOffset = 0;

		unsigned long long EntriesTotalSize = 0;
		for (const auto& file : AssetAndSourceFileList)
		{
			GAssetPackEntry Entry;

			Entry.FilePath = std::filesystem::relative(file, assetLoader->GetAssetDirectory()).string();
			GUtil::StandardizePath(Entry.FilePath);
			Entry.PathLength = (unsigned int)Entry.FilePath.size() + 1;
			Entry.FileSize = std::filesystem::file_size(file);
			EntryList.push_back(Entry);

			EntriesTotalSize += sizeof(GAssetPackEntry::PathLength) + Entry.PathLength +
				sizeof(GAssetPackEntry::FileSize) + sizeof(GAssetPackEntry::DataOffset);
		}
		
		CurrentDataOffset += sizeof(GAssetPackHeader) + EntriesTotalSize;

		for (auto& entry : EntryList)
		{
			entry.DataOffset = CurrentDataOffset;
			CurrentDataOffset += entry.FileSize;
		}

		std::ofstream PackOutputFile(packageFilePath, std::ios::binary);

		GAssetPackHeader FileHeader;
		FileHeader.FileCount = (unsigned int)EntryList.size();
		FileHeader.EntriesOffset = sizeof(GAssetPackHeader);
		PackOutputFile.write((const char*)&FileHeader, sizeof(GAssetPackHeader));

		for (const auto& entry : EntryList)
		{
			PackOutputFile.write((const char*)&entry.PathLength, sizeof(GAssetPackEntry::PathLength));
			PackOutputFile.write(entry.FilePath.c_str(), entry.PathLength);
			PackOutputFile.write((const char*)&entry.FileSize, sizeof(GAssetPackEntry::FileSize));
			PackOutputFile.write((const char*)&entry.DataOffset, sizeof(GAssetPackEntry::DataOffset));
		}

		for (unsigned long long i = 0; i < EntryList.size(); i++)
		{
			const auto& file = AssetAndSourceFileList[i];
			const auto& entry = EntryList[i];

			std::ifstream AssetInputFile;
			AssetInputFile.open(file, std::ios::binary);

			if (!AssetInputFile.is_open())
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("Failed to read asset file : '{}'!", file));
			}

			PackOutputFile.seekp(entry.DataOffset);

			std::vector<char> FileBuffer(entry.FileSize);
			AssetInputFile.read(FileBuffer.data(), entry.FileSize);
			PackOutputFile.write(FileBuffer.data(), entry.FileSize);
		}

		PackOutputFile.close();
	}

	void GAssetPacker::LoadAssetPackage(std::shared_ptr<GAssetLoader> assetLoader, const std::string& packageFilePath)
	{
		GUARDIAN_CHECK_POINTER(assetLoader);

		std::ifstream PackInputFile(packageFilePath, std::ios::binary);
		if (!PackInputFile.is_open())
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Failed to load asset package : '{}' !", packageFilePath));
		}

		GAssetPackHeader FileHeader;
		PackInputFile.read((char*)&FileHeader, sizeof(GAssetPackHeader));

		if (std::strncmp(FileHeader.Identifier, "GEPAK", 6) != 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Invalid asset package : '{}'!", packageFilePath));
		}

		PackInputFile.seekg(FileHeader.EntriesOffset);
		std::vector<GAssetPackEntry> EntryList(FileHeader.FileCount);

		for (unsigned int i = 0; i < FileHeader.FileCount; i++)
		{
			GAssetPackEntry Entry;

			PackInputFile.read((char*)&Entry.PathLength, sizeof(GAssetPackEntry::PathLength));

			Entry.FilePath.resize(Entry.PathLength - 1);
			PackInputFile.read(&Entry.FilePath[0], Entry.PathLength - 1);

			PackInputFile.seekg(1, std::ios_base::cur);

			PackInputFile.read((char*)&Entry.FileSize, sizeof(GAssetPackEntry::FileSize));
			PackInputFile.read((char*)&Entry.DataOffset, sizeof(GAssetPackEntry::DataOffset));

			EntryList[i] = Entry;
		}

		assetLoader->ClearAllAsset();

		std::vector<std::string> AssetFileList;
		for (const auto& entry : EntryList)
		{
			if (GUtil::GetFileExtension(entry.FilePath) == ".gasset")
			{
				AssetFileList.push_back(entry.FilePath);
			}
		}

		for (const auto& assetFile : AssetFileList)
		{
			char* AssetFileData = null;
			unsigned long long AssetFileSize = 0;
			char* SourceFileData = null;
			unsigned long long SourceFileSize = 0;
			
			std::shared_ptr<GAsset> asset = std::make_shared<GAsset>();

			try
			{
				GAssetPackEntry AssetFileEntry;
				for (const auto& entry : EntryList)
				{
					if (entry.FilePath == assetFile)
					{
						AssetFileEntry = entry;
						AssetFileData = new char[entry.FileSize];
						AssetFileSize = entry.FileSize;
						break;
					}
				}

				PackInputFile.seekg(AssetFileEntry.DataOffset);
				PackInputFile.read(AssetFileData, AssetFileSize);

				std::stringstream AssetFileString;
				AssetFileString.write(AssetFileData, AssetFileSize);
				YAML::Node AssetFileNode = YAML::Load(AssetFileString);
				GAssetSerializer::Deserialize(asset, AssetFileNode);

				const auto& SourceFilePath = std::filesystem::relative(asset->GetAssetSourcePath(), assetLoader->GetAssetDirectory()).string();
				GAssetPackEntry SourceFileEntry;
				for (const auto& entry : EntryList)
				{
					if (entry.FilePath == SourceFilePath)
					{
						SourceFileEntry = entry;
						SourceFileData = new char[entry.FileSize];
						SourceFileSize = entry.FileSize;
						break;
					}
				}

				PackInputFile.seekg(SourceFileEntry.DataOffset);
				PackInputFile.read(SourceFileData, SourceFileSize);

				asset->LoadAssetData(asset->GetAssetSourcePath(), SourceFileData, SourceFileSize);

				assetLoader->LoadedAssetList[asset->GetAssetName()] = asset;
			}
			catch (...)
			{
				if (AssetFileData)
				{
					delete[] AssetFileData;
					AssetFileData = null;
				}

				if (SourceFileData)
				{
					delete[] SourceFileData;
					SourceFileData = null;
				}

				throw;
			}

			if (AssetFileData)
			{
				delete[] AssetFileData;
				AssetFileData = null;
			}

			if (SourceFileData)
			{
				delete[] SourceFileData;
				SourceFileData = null;
			}
		}
	}

	void GAssetPacker::UnpackAsset(const std::string& packageFilePath, const std::string& unpackDirectory)
	{
		std::filesystem::create_directories(unpackDirectory);

		std::ifstream PackInputFile(packageFilePath, std::ios::binary);
		if (!PackInputFile.is_open())
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Failed to load asset package : '{}' !", packageFilePath));
		}

		GAssetPackHeader FileHeader;
		PackInputFile.read((char*)&FileHeader, sizeof(GAssetPackHeader));

		if (std::strncmp(FileHeader.Identifier, "GEPAK", 6) != 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Invalid asset package : '{}'!", packageFilePath));
		}

		PackInputFile.seekg(FileHeader.EntriesOffset);
		std::vector<GAssetPackEntry> EntryList(FileHeader.FileCount);

		for (unsigned int i = 0; i < FileHeader.FileCount; i++)
		{
			GAssetPackEntry Entry;

			PackInputFile.read((char*)&Entry.PathLength, sizeof(GAssetPackEntry::PathLength));

			Entry.FilePath.resize(Entry.PathLength - 1);
			PackInputFile.read(&Entry.FilePath[0], Entry.PathLength - 1);

			PackInputFile.seekg(1, std::ios_base::cur);

			PackInputFile.read((char*)&Entry.FileSize, sizeof(GAssetPackEntry::FileSize));
			PackInputFile.read((char*)&Entry.DataOffset, sizeof(GAssetPackEntry::DataOffset));

			EntryList[i] = Entry;
		}

		for (const auto& entry : EntryList)
		{
			std::string TargetPath = GUtil::ExtendDirectory(unpackDirectory, entry.FilePath);
			std::filesystem::create_directories(std::filesystem::path(TargetPath).parent_path());

			std::ofstream AssetOutputFile;
			AssetOutputFile.open(TargetPath, std::ios::binary);

			PackInputFile.seekg(entry.DataOffset);

			std::vector<char> FileBuffer(entry.FileSize);
			PackInputFile.read(FileBuffer.data(), entry.FileSize);
			AssetOutputFile.write(FileBuffer.data(), entry.FileSize);

			AssetOutputFile.close();
		}

		PackInputFile.close();
	}
}
