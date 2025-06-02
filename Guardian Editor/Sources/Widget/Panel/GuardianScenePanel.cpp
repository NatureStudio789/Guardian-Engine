#include "GuardianScenePanel.h"
#include <Graphics/Renderer/GuardianRenderer.h>
#include <Application/GuardianApplication.h>
#include <GuardianEngine.h>

namespace GE
{
	GuardianScenePanel::GuardianScenePanel()
	{
		this->PanelName = "Scene";
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->SelectedEntityId = 0;
		this->PlayButtonTexture.InitializeTexture(GuardianSurface("../Guardian Engine/Assets/Textures/PlayButton.png"));
		this->StopButtonTexture.InitializeTexture(GuardianSurface("../Guardian Engine/Assets/Textures/StopButton.png"));
	}

	GuardianScenePanel::GuardianScenePanel(GuardianScene* scene)
	{
		this->PanelName = "Scene";
		this->PanelScene = scene;
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->SelectedEntityId = 0;
		this->PlayButtonTexture.InitializeTexture(GuardianSurface("../Guardian Engine/Assets/Textures/PlayButton.png"));
		this->StopButtonTexture.InitializeTexture(GuardianSurface("../Guardian Engine/Assets/Textures/StopButton.png"));
	}

	GuardianScenePanel::GuardianScenePanel(const GuardianScenePanel& other)
	{
		this->PanelName = other.PanelName;
		this->SelectedEntityId = other.SelectedEntityId;
		this->CurrentOperation = other.CurrentOperation;
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

	void GuardianScenePanel::SetCurrentOperation(int operation)
	{
		this->CurrentOperation = operation;
	}

	void GuardianScenePanel::Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Depth");

		for (auto& map : GuardianRenderer::RenderingSceneGraphList[this->PanelScene->SceneName]->DepthGraphList)
		{
			ImGui::Image((ImTextureID)map.second->GetGraphDepthStencil()->GetDepthStencilShaderResource().Get(),
				ImVec2(1024, 1024));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(this->PanelScene->SceneName.c_str());

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

			GuardianRenderer::GetRenderingSceneGraph(this->PanelScene)->ResizeSceneGraph(
				(int)CurrentScenePanelSize.x, (int)CurrentScenePanelSize.y);
			this->PanelScene->UpdateProjectionAspect(CurrentScenePanelSize.x, CurrentScenePanelSize.y);
			IsFirst = false;
		}

		ImGui::Image((ImTextureID)GuardianRenderer::GetRenderingSceneGraph(this->PanelScene)->GetSceneGraphFramebuffer()->
			GetFramebufferResource().Get(), ImGui::GetContentRegionAvail());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCES_BROWSER_ITEM"))
			{
				if (std::filesystem::path(GString((const char*)payload->Data)).extension().string() == ".gscene")
				{
					this->PanelScene->LoadSceneAs(GString((const char*)payload->Data));
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleVar();

		if (this->PanelScene->GetEntity(this->SelectedEntityId) && 
			this->PanelScene->GetEntity(this->SelectedEntityId)->HasComponent<GuardianTransformComponent>())
		{
			if (this->PanelScene->GetSceneState() == GE_SCENE_EDIT)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

				XMFLOAT4X4 view;
				XMStoreFloat4x4(&view, this->PanelScene->EditorCamera->GetViewMatrix());
				XMFLOAT4X4 projection;
				XMStoreFloat4x4(&projection, this->PanelScene->EditorCamera->GetProjectionMatrix());
				XMFLOAT4X4 transform;
				XMStoreFloat4x4(&transform, this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().GetTransformMatrix());

				ImGuizmo::Manipulate((const float*)view.m, (const float*)projection.m, (ImGuizmo::OPERATION)this->CurrentOperation,
					ImGuizmo::LOCAL, (float*)transform.m);
				
				if (ImGuizmo::IsUsing())
				{
					if (this->CurrentOperation == ImGuizmo::TRANSLATE)
					{
						XMVECTOR VScale, VTranslation, VRotation;
						XMMatrixDecompose(&VScale, &VRotation, &VTranslation, XMLoadFloat4x4(&transform));
						XMFLOAT3 Position;
						XMStoreFloat3(&Position, VTranslation);

						this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Position =
							GVector3(Position.x, Position.y, Position.z);
					}
					else if (this->CurrentOperation == ImGuizmo::ROTATE)
					{
						XMVECTOR VScale, VTranslation, VRotation;
						XMMatrixDecompose(&VScale, &VRotation, &VTranslation, XMLoadFloat4x4(&transform));
						XMFLOAT4 Quaternion;
						XMStoreFloat4(&Quaternion, VRotation);

						this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Rotation =
							GVector4::QuaternionToEuler(GVector4(Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w));
					}
					else if (this->CurrentOperation == ImGuizmo::SCALE)
					{
						XMVECTOR VScale, VTranslation, VRotation;
						XMMatrixDecompose(&VScale, &VRotation, &VTranslation, XMLoadFloat4x4(&transform));
						XMFLOAT3 Scale;
						XMStoreFloat3(&Scale, VScale);

						this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Scale =
							GVector3(Scale.x, Scale.y, Scale.z);
					}
				}
			}
		}

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
