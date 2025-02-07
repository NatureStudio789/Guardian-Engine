#include "GuardianScenePanel.h"
#include <Application/GuardianApplication.h>

namespace guardian
{
	GuardianScenePanel::GuardianScenePanel()
	{
		this->PanelName = "Scene";
		this->SelectedEntityId = 0;
		this->PlayButtonTexture.InitializeTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/PlayButton.png"));
		this->StopButtonTexture.InitializeTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/StopButton.png"));
	}

	GuardianScenePanel::GuardianScenePanel(GuardianScene* scene)
	{
		this->PanelName = "Scene";
		this->PanelScene = scene;
		this->SelectedEntityId = 0;
		this->PlayButtonTexture.InitializeTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/PlayButton.png"));
		this->StopButtonTexture.InitializeTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/StopButton.png"));
	}

	GuardianScenePanel::GuardianScenePanel(const GuardianScenePanel& other)
	{
		this->PanelName = other.PanelName;
		this->SelectedEntityId = other.SelectedEntityId;
		this->PlayButtonTexture = other.PlayButtonTexture;
		this->StopButtonTexture = other.StopButtonTexture;
	}

	GuardianScenePanel::~GuardianScenePanel()
	{
		this->PanelScene = null;
		this->SelectedEntityId = 0;
	}

	void GuardianScenePanel::SetSelectedEntityId(GuardianUUID entityId)
	{
		this->SelectedEntityId = entityId;
	}

	void GuardianScenePanel::Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene");

		if (ImGui::IsWindowFocused())
		{
			this->PanelScene->ShouldOperateCamera = true;
		}
		else
		{
			this->PanelScene->ShouldOperateCamera = false;
		}

		ImVec2 CurrentScenePanelSize = ImGui::GetContentRegionAvail();
		static GVector2 LastScenePanelSize = { CurrentScenePanelSize.x, CurrentScenePanelSize.y };
		static bool IsFirst = true;
		if (LastScenePanelSize != GVector2(CurrentScenePanelSize.x, CurrentScenePanelSize.y) || IsFirst)
		{
			LastScenePanelSize = { CurrentScenePanelSize.x, CurrentScenePanelSize.y };

			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->UpdateSceneGraphicsResolution(
				(int)CurrentScenePanelSize.x, (int)CurrentScenePanelSize.y);
			this->PanelScene->UpdateProjectionAspect(CurrentScenePanelSize.x, CurrentScenePanelSize.y);
			IsFirst = false;
		}

		ImGui::Image((ImTextureID)
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->
			GetGraphicsSceneFramebuffer()->GetFramebufferResource().Get(), ImGui::GetContentRegionAvail());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCES_BROWSER_ITEM"))
			{
				this->PanelScene->LoadSceneAs(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
					GString((const char*)payload->Data));
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.305f, 0.3f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.105f, 0.1f, 1.0f));
		ImGui::Begin("##toolbar", null,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetContentRegionAvail().y - 4.0f;
		GuardianTexture icon = this->PanelScene->SceneState == GE_SCENE_EDIT ? this->PlayButtonTexture : this->StopButtonTexture;
		ImGui::SameLine((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton("PlayButton",
			(ImTextureID)icon.GetTextureResource().Get(), ImVec2(size, size)))
		{
			if (this->PanelScene->SceneState == GE_SCENE_RUNTIME)
			{
				this->PanelScene->StopRuntime();
			}
			else if (this->PanelScene->SceneState == GE_SCENE_EDIT)
			{
				this->PanelScene->StartRuntime();
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::End();
	}
}
