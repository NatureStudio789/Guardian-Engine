#include "GuardianSceneHierarchyPanel.h"
#include <ECS/Entity/GuardianEntity.h>
#include <Application/GuardianApplication.h>

namespace guardian
{
	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel()
	{
		this->PanelName = "Scene Hierarchy";
		this->PanelScene = null;
		this->SelectedEntityId = 0;
	}

	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel(GuardianScene* scene)
	{
		this->PanelName = "Scene Hierarchy";
		this->SelectedEntityId = 0;
		this->SetScene(scene);
	}

	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel(const GuardianSceneHierarchyPanel& other)
	{
		this->PanelScene = other.PanelScene;
		this->SelectedEntityId = other.SelectedEntityId;
		this->PanelName = other.PanelName;
	}

	GuardianSceneHierarchyPanel::~GuardianSceneHierarchyPanel()
	{
		this->PanelScene = null;
		this->SelectedEntityId = 0;
	}

	void GuardianSceneHierarchyPanel::SetScene(GuardianScene* scene)
	{
		this->PanelScene = scene;
	}

	void GuardianSceneHierarchyPanel::Render()
	{
		static bool open = true;
		if (open)
		{
			ImGui::Begin("Scene Hierarchy", &open);

			std::vector<entt::entity> DeletedEntities;
			for (auto& entity : this->PanelScene->SceneEntityList)
			{
				if (!RenderEntityNode(entity.second.get()))
				{
					DeletedEntities.push_back(entity.first);
				}
			}

			for (auto& handle : DeletedEntities)
			{
				this->PanelScene->RemoveEntity(handle);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				this->SelectedEntityId = 0;
			}

			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					this->PanelScene->CreateEntity("Empty Entity");
				}

				ImGui::EndPopup();
			}

			ImGui::End();
		}

		static bool open1 = true;
		if (open1)
		{
			ImGui::Begin("Properties", &open1);

			this->RenderEntityComponents();

			ImGui::End();
		}
	}

	GuardianUUID GuardianSceneHierarchyPanel::GetSelectedEntityId()
	{
		return this->SelectedEntityId;
	}

	bool GuardianSceneHierarchyPanel::RenderEntityNode(GuardianEntity* entity)
	{
		ImGuiTreeNodeFlags flags = ((this->SelectedEntityId == entity->GetEntityId()) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(*entity), flags,
			entity->GetComponent<GuardianTagComponent>().Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			this->SelectedEntityId = entity->GetEntityId();
		}

		bool EntityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				EntityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			

			ImGui::TreePop();
		}

		if (EntityDeleted)
		{
			this->SelectedEntityId = 0;
			return false;
		}
		else
		{
			return true;
		}
	}

	void GuardianSceneHierarchyPanel::RenderEntityComponents()
	{
		if (auto SelectedEntity = this->PanelScene->GetEntity(this->SelectedEntityId))
		{
			if (SelectedEntity->HasComponent<GuardianTagComponent>())
			{
				auto& tag = SelectedEntity->GetComponent<GuardianTagComponent>().Tag;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianTagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Tag Component"))
				{
					char Buffer[256];
					memset(Buffer, 0, sizeof(Buffer));
					strcpy_s(Buffer, tag.c_str());
					if (ImGui::InputText("Tag", Buffer, sizeof(Buffer)))
					{
						tag = GString(Buffer);
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianTransformComponent>())
			{
				auto& transform = SelectedEntity->GetComponent<GuardianTransformComponent>();

				if (ImGui::TreeNodeEx((void*)typeid(GuardianTransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Transform Component"))
				{

					float position[3] = { transform.Position.x, transform.Position.y, transform.Position.z };
					if (ImGui::DragFloat3("Position", position, 0.1f))
					{
						transform.Position = GVector3(position[0], position[1], position[2]);
					}

					float rotation[3] = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
					if (ImGui::DragFloat3("Rotation", rotation, 1.0f))
					{
						transform.Rotation = GVector3(rotation[0], rotation[1], rotation[2]);
					}

					if (!SelectedEntity->HasComponent<GuardianCameraComponent>())
					{
						float scale[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };
						if (ImGui::DragFloat3("Scale", scale, 0.1f, 0.0f))
						{
							transform.Scale = GVector3(scale[0], scale[1], scale[2]);
						}
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianCameraComponent>()) 
			{
				auto& camera = SelectedEntity->GetComponent<GuardianCameraComponent>();

				if (ImGui::TreeNodeEx((void*)typeid(GuardianCameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Camera Component"))
				{
					ImGui::Text("Perspective Projection");

					ImGui::DragFloat("FOV", &camera.Projection.FOV, 0.5f, 10.0f, 180.0f);

					ImGui::DragFloat("NearZ", &camera.Projection.NearZ, 0.001f, 0.001f, 1.0f);
					ImGui::DragFloat("FarZ", &camera.Projection.FarZ, 1.0f, 1.5f, 0.0f);

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianScriptComponent>())
			{
				auto& script = SelectedEntity->GetComponent<GuardianScriptComponent>();

				bool IsScriptExists = GuardianScriptEngine::IsEntityClassExists(script.ClassName);

				if (ImGui::TreeNodeEx((void*)typeid(GuardianScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Script Component"))
				{
					if (!IsScriptExists)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));
					}

					char buffer[64];
					strcpy_s(buffer, script.ClassName.c_str());
					if (ImGui::InputText("Class", buffer, sizeof(buffer)))
					{
						script.ClassName = buffer;
					}

					if (!IsScriptExists)
					{
						ImGui::PopStyleColor();
					}

					ImGui::TreePop();
				}
			}

			if (SelectedEntity->HasComponent<GuardianModelComponent>())
			{
				auto& model = SelectedEntity->GetComponent<GuardianModelComponent>();

				if (ImGui::TreeNodeEx((void*)typeid(GuardianModelComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Model Component"))
				{
					ImGui::Text(("Model File Path : " + model.GetModelFilePath()).c_str());

					ImGui::TreePop();
				}
			}

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Transform Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianTransformComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Camera Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianCameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Script Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianScriptComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Model Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianModelComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Box Collider Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianBoxColliderComponent>().BoxCollider->InitializeBoxCollider(GuardianBoxColliderProperties(), 
							GuardianPhysicsMaterial(0.5f, 0.5f, 0.5f));
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Dynamic RigidBody Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianDynamicRigidBodyComponent>().DynamicRigidBody->
						InitializeDynamicRigidBody(
							this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->GetComponent<GuardianTransformComponent>());
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	}
}
