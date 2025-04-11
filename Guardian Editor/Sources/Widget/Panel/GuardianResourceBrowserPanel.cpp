#include "GuardianResourceBrowserPanel.h"
#include <Application/GuardianApplication.h>

namespace GE
{
	GuardianResourceBrowserPanel::GuardianResourceBrowserPanel()
	{
		this->PanelName = "Resource Browser";
		this->ResourceDirectory = "Assets";
		this->CurrentDirectory = this->ResourceDirectory;

		this->FolderIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Assets/Textures/FolderIcon.png"));
		this->ModelFileIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Assets/Textures/ModelFileIcon.png"));
		this->TextureFileIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Assets/Textures/TextureFileIcon.png"));
		this->DefaultFileIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Assets/Textures/DefaultFileIcon.png"));
	}

	GuardianResourceBrowserPanel::GuardianResourceBrowserPanel(const GString& resourcesDir)
	{
		this->SetResourceDirectory(resourcesDir);
	}

	void GuardianResourceBrowserPanel::SetResourceDirectory(const GString& resourcesDir)
	{
		this->ResourceDirectory = resourcesDir;
		this->CurrentDirectory = this->ResourceDirectory;
	}

	void GuardianResourceBrowserPanel::Render()
	{
		static bool open = true;
		if (open)
		{
			ImGui::Begin("Resources Browser", &open, ImGuiWindowFlags_MenuBar);

			this->RenderMenuBar();

			this->RenderDirectory(this->CurrentDirectory);

			ImGui::End();
		}
	}

	void GuardianResourceBrowserPanel::RenderMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{


				ImGui::EndMenu();
			}

			if (this->CurrentDirectory != this->ResourceDirectory)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.4f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.5f, 0.5f, 1.0f));
				if (ImGui::Button("Back"))
				{
					this->CurrentDirectory = std::filesystem::path(this->CurrentDirectory).parent_path().string();
				}
				ImGui::PopStyleColor(3);
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
			ImGui::Button((this->CurrentDirectory + "\\").c_str());
			ImGui::PopStyleColor(3);

			ImGui::EndMenuBar();
		}
	}

	void GuardianResourceBrowserPanel::RenderDirectory(const GString& directory)
	{
		static float padding = 16.0f;
		static float IconSize = 64.0f;
		float CellSize = IconSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int ColumnCount = (int)(panelWidth / CellSize);
		if (ColumnCount < 1)
		{
			ColumnCount = 1;
		}

		ImGui::Columns(ColumnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

			auto icon = this->SetFileUnitIcon(directoryEntry);
			auto filename = directoryEntry.path().filename().string();
			if (directoryEntry.path().extension().string() == ".obj" ||
				directoryEntry.path().extension().string() == ".fbx")
			{
				filename = filename.substr(0, filename.find_last_of('.'));
			}
			ImGui::ImageButton(filename.c_str(), (ImTextureID)icon.Get(), ImVec2(IconSize, IconSize));
			ImGui::PopStyleColor(3);

			if (ImGui::BeginDragDropSource())
			{
				GString path = directoryEntry.path().string();
				ImGui::SetDragDropPayload("RESOURCES_BROWSER_ITEM", 
					(const void*)path.c_str(), (path.size() + 1) * sizeof(char), ImGuiCond_Once);

				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					this->CurrentDirectory = directoryEntry.path().string();
				}
			}

			ImGui::TextWrapped(filename.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianResourceBrowserPanel::SetFileUnitIcon(std::filesystem::directory_entry directoryEntry)
	{
		if (directoryEntry.is_directory())
		{
			return this->FolderIcon.GetTextureResource();
		}
		else
		{
			const auto& path = directoryEntry.path();

			if (path.extension().string() == ".png" ||
				path.extension().string() == ".jpg")
			{
				return GuardianAssetSystem::GetTexture(path.filename().string()).GetTextureResource();
			}
			else if (path.extension().string() == ".obj" || path.extension().string() == ".fbx")
			{
				return GuardianAssetSystem::GetMeshAssetRenderingViewFromPath(path.string())->GetFramebufferResource();
			}
			else
			{
				return this->DefaultFileIcon.GetTextureResource();
			}
		}
	}
}
