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
		GuardianEngine::EngineInstance->GetScene() = null;
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

		for (auto& map : GuardianRenderer::RenderingSceneGraphList[GuardianEngine::EngineInstance->GetScene()->SceneName]->DepthGraphList)
		{
			ImGui::Image((ImTextureID)map.second->GetGraphDepthStencil()->GetDepthStencilShaderResource().Get(),
				ImVec2(1024, 1024));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Scene");

		if (ImGui::IsWindowFocused())
		{
			GuardianEngine::EngineInstance->GetScene()->ShouldOperateCamera = true;
		}
		else
		{
			GuardianEngine::EngineInstance->GetScene()->ShouldOperateCamera = false;
		}

		ImVec2 CurrentScenePanelSize = ImGui::GetContentRegionAvail();
		static GVector2 LastScenePanelSize = { CurrentScenePanelSize.x, CurrentScenePanelSize.y };
		static bool IsFirst = true;
		if (LastScenePanelSize != GVector2(CurrentScenePanelSize.x, CurrentScenePanelSize.y) || IsFirst)
		{
			LastScenePanelSize = { CurrentScenePanelSize.x, CurrentScenePanelSize.y };

			GuardianRenderer::GetRenderingSceneGraph(GuardianEngine::EngineInstance->GetScene().get())->ResizeSceneGraph(
				(int)CurrentScenePanelSize.x, (int)CurrentScenePanelSize.y);
			GuardianEngine::EngineInstance->GetScene()->UpdateProjectionAspect(CurrentScenePanelSize.x, CurrentScenePanelSize.y);
			IsFirst = false;
		}

		ImGui::Image((ImTextureID)GuardianRenderer::GetRenderingSceneGraph(GuardianEngine::EngineInstance->GetScene().get())->GetSceneGraphFramebuffer()->
			GetFramebufferResource().Get(), ImGui::GetContentRegionAvail());

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCES_BROWSER_ITEM"))
			{
				GuardianEngine::EngineInstance->SetCurrentScene(GString((const char*)payload->Data));
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleVar();

		if (GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId) && 
			GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId)->HasComponent<GuardianTransformComponent>())
		{
			if (GuardianEngine::EngineInstance->GetScene()->GetSceneState() == GE_SCENE_EDIT)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

				XMFLOAT4X4 view;
				XMStoreFloat4x4(&view, GuardianEngine::EngineInstance->GetScene()->EditorCamera->GetViewMatrix());
				XMFLOAT4X4 projection;
				XMStoreFloat4x4(&projection, GuardianEngine::EngineInstance->GetScene()->EditorCamera->GetProjectionMatrix());
				XMFLOAT4X4 transform;
				XMStoreFloat4x4(&transform, GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().GetTransformMatrix());

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

						GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Position =
							GVector3(Position.x, Position.y, Position.z);
					}
					else if (this->CurrentOperation == ImGuizmo::ROTATE)
					{
						XMVECTOR VScale, VTranslation, VRotation;
						XMMatrixDecompose(&VScale, &VRotation, &VTranslation, XMLoadFloat4x4(&transform));
						XMFLOAT4 Quaternion;
						XMStoreFloat4(&Quaternion, VRotation);

						GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Rotation =
							GVector4::QuaternionToEuler(GVector4(Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w));
					}
					else if (this->CurrentOperation == ImGuizmo::SCALE)
					{
						XMVECTOR VScale, VTranslation, VRotation;
						XMMatrixDecompose(&VScale, &VRotation, &VTranslation, XMLoadFloat4x4(&transform));
						XMFLOAT3 Scale;
						XMStoreFloat3(&Scale, VScale);

						GuardianEngine::EngineInstance->GetScene()->GetEntity(this->SelectedEntityId)->GetComponent<GuardianTransformComponent>().Scale =
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
		GuardianTexture icon = GuardianEngine::EngineInstance->GetScene()->SceneState == GE_SCENE_EDIT ? this->PlayButtonTexture : this->StopButtonTexture;
		ImGui::SameLine((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton("PlayButton",
			(ImTextureID)icon.GetTextureResource().Get(), ImVec2(size, size)))
		{
			if (GuardianEngine::EngineInstance->GetScene()->SceneState == GE_SCENE_RUNTIME)
			{
				GuardianEngine::EngineInstance->GetScene()->StopRuntime();
			}
			else if (GuardianEngine::EngineInstance->GetScene()->SceneState == GE_SCENE_EDIT)
			{
				GuardianEngine::EngineInstance->GetScene()->StartRuntime();
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::End();
	}
}
