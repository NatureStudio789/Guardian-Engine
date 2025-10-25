#include "GAssetEditor.h"

namespace GE
{
	void GAssetEditor::Initialize()
	{
		{
			this->AssetBrowserPanel = std::make_shared<EUI::GPanel>("Asset Browser");

			this->AddWidgetToEditor(this->AssetBrowserPanel);
		}

		this->AssetDirectory = GProject::Instance->GetFullAssetDirectory();
		this->CurrentDirectory = this->AssetDirectory;

		this->DirectoryTexture = GTexture::CreateNewTexture(null,
			*GAssetLoaderRegistry::GetAssetLoader("EditorAssetLoader")->GetAsset("Directory")->GetAssetData<std::shared_ptr<GSurface>>().get());
		this->FileTexture = GTexture::CreateNewTexture(null, 
			*GAssetLoaderRegistry::GetAssetLoader("EditorAssetLoader")->GetAsset("File")->GetAssetData<std::shared_ptr<GSurface>>().get());
		this->BackTexture = GTexture::CreateNewTexture(null,
			*GAssetLoaderRegistry::GetAssetLoader("EditorAssetLoader")->GetAsset("Back")->GetAssetData<std::shared_ptr<GSurface>>().get());
	}

	void GAssetEditor::Update()
	{
		this->AssetBrowserPanel->ClearPanelWidgets();

		auto& MenuBar = std::make_shared<EUI::GMenuBar>();
		this->AssetBrowserPanel->AddWidgetToPanel(MenuBar);
		this->AssetBrowserPanel->AddPanelFlag(EUI::GPanel::GEPanelFlag_MenuBar);

		static std::string Directory = this->CurrentDirectory;

		MenuBar->AddWidgetToMenuBar(std::make_shared<EUI::GImageButton>("Back", [=]()
			{
				if (!GUtil::CompareFileDirectory(this->CurrentDirectory, this->AssetDirectory))
				{
					this->CurrentDirectory = std::filesystem::path(this->CurrentDirectory).parent_path().string();
					Directory = this->CurrentDirectory;
				}
			}, (EUI::GImage::Id)this->BackTexture->GetViewDescriptorHandle()->GPUHandle.ptr, GVector2(20.0f, 20.0f)));

		MenuBar->AddWidgetToMenuBar(std::make_shared<EUI::GInputBox>("Path", &Directory));
		if (std::filesystem::exists(Directory) && std::filesystem::is_directory(Directory))
		{
			this->CurrentDirectory = Directory;
		}
		else
		{
			Directory = this->CurrentDirectory;
		}

		static float padding = 20.0f;
		static float IconSize = 64.0f;
		float CellSize = IconSize + padding;

		static float panelWidth = ImGui::GetContentRegionAvail().x;
		this->AssetBrowserPanel->SetWidgetEventProcessFunction([]()
			{
				panelWidth = ImGui::GetContentRegionAvail().x;
			});
		int ColumnCount = (int)(panelWidth / CellSize);
		if (ColumnCount < 1)
		{
			ColumnCount = 1;
		}

		auto& ColumnLayout = std::make_shared<EUI::GColumnLayout>();
		ColumnLayout->SetColumnCount(ColumnCount);
		this->AssetBrowserPanel->AddWidgetToPanel(ColumnLayout);
		for (const auto& directoryEntry : std::filesystem::directory_iterator(this->CurrentDirectory))
		{
			EUI::GImage::Id ImageId = 0;

			const auto& Path = directoryEntry.path();
			if (directoryEntry.is_directory())
			{
				ImageId = (unsigned long long)this->DirectoryTexture->GetViewDescriptorHandle()->GPUHandle.ptr;

				auto& DirectoryButton = std::make_shared<EUI::GImageButton>(GUtil::GetDirectoryName(Path.string()), [=]()
					{
						/*this->CurrentDirectory = Path.string();
						Directory = this->CurrentDirectory;*/
					}, ImageId, GVector2(IconSize, IconSize));

				DirectoryButton->SetWidgetEventProcessFunction([=]()
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
						{
							this->CurrentDirectory = Path.string();
							Directory = this->CurrentDirectory;
						}
					});

				ColumnLayout->AddColumn({ 
					DirectoryButton, 
					std::make_shared<EUI::GText>(GUtil::GetDirectoryName(Path.string()), GVector4{1.0f, 1.0f, 1.0f, 1.0f}, true) });
			}
			else
			{
				ImageId = (unsigned long long)this->FileTexture->GetViewDescriptorHandle()->GPUHandle.ptr;

				ColumnLayout->AddColumn({
					std::make_shared<EUI::GImageButton>(Path.filename().string(), [=]()
					{
						
					}, ImageId, GVector2(IconSize, IconSize)),

					std::make_shared<EUI::GText>(Path.filename().string(), GVector4{1.0f, 1.0f, 1.0f, 1.0f}, true) });
			}
		}
	}
}
