#include "GuardianSceneHierarchyPanel.h"
#include <ECS/Entity/GuardianEntity.h>
#include <Application/GuardianApplication.h>

namespace GE
{
	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel()
	{
		this->PanelName = "Scene Hierarchy";
		this->PanelScene = null;
		this->SelectedEntityId = 0;
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->MeshFileIcon = GuardianTexture(GuardianSurface("../Guardian Engine/Assets/Textures/MeshFileIcon.png"));
	}

	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel(GuardianScene* scene)
	{
		this->PanelName = "Scene Hierarchy";
		this->SelectedEntityId = 0;
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->MeshFileIcon = GuardianTexture(GuardianSurface("../Guardian Engine/Assets/Textures/MeshFileIcon.png"));
		this->SetScene(scene);
	}

	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel(const GuardianSceneHierarchyPanel& other)
	{
		this->PanelScene = other.PanelScene;
		this->SelectedEntityId = other.SelectedEntityId;
		this->PanelName = other.PanelName;
		this->CurrentOperation = other.CurrentOperation;
		this->MeshFileIcon = other.MeshFileIcon;
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
			bool nodeOpened = ImGui::TreeNodeEx(this->PanelScene->SceneName.c_str(), flags);

			if (nodeOpened)
			{
				for (auto& entity : this->PanelScene->SceneEntityList)
				{
					if (!RenderEntityNode(entity.second.get()))
					{
						DeletedEntities.push_back(entity.first);
					}
				}

				ImGui::TreePop();
			}

			for (auto& handle : DeletedEntities)
			{
				this->PanelScene->RemoveEntity(handle);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				if (auto selected = this->PanelScene->GetEntity(this->SelectedEntityId))
				{
					selected->UnselectEntity();
				}
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
		static bool OpenMeshBrowser = false;
		static bool OpenMaterialBrowser = false;
		static bool OpenTextureBrowser = false;
		static int SelectedTextureIndex = -1;
		if (open1)
		{
			ImGui::Begin("Properties", &open1);

			this->RenderEntityComponents(OpenMeshBrowser, OpenTextureBrowser, SelectedTextureIndex, OpenMaterialBrowser);

			ImGui::End();
		}

		GString meshName;
		this->RenderMeshBrowser(meshName, OpenMeshBrowser);
		if (!meshName.empty())
		{
			auto& meshData = GuardianAssetSystem::GetMeshAsset(meshName).GetMeshAssetData();
			this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().Mesh = std::make_shared<GuardianMesh>();
			this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().Mesh->InitializeMesh(meshName, meshData);
		}
	}

	GuardianUUID GuardianSceneHierarchyPanel::GetSelectedEntityId()
	{
		return this->SelectedEntityId;
	}

	const int GuardianSceneHierarchyPanel::GetCurrentOperation() const
	{
		return this->CurrentOperation;
	}

	bool GuardianSceneHierarchyPanel::RenderEntityNode(GuardianEntity* entity)
	{
		ImGuiTreeNodeFlags flags = ((this->SelectedEntityId == entity->GetEntityId()) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(*entity), flags,
			entity->GetComponent<GuardianTagComponent>().Tag.c_str());
		if (ImGui::IsItemClicked())
		{
			if (auto selected = this->PanelScene->GetEntity(this->SelectedEntityId))
			{
				selected->UnselectEntity();
			}

			this->SelectedEntityId = entity->GetEntityId();
			entity->SelectEntity();
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
			entity->UnselectEntity();
			this->SelectedEntityId = 0;
			return false;
		}
		else
		{
			return true;
		}
	}

	void GuardianSceneHierarchyPanel::RenderEntityComponents(bool& openMeshBrowser, 
		bool& openTextureBrowser, int& textureIndex, bool& openMaterialBrowser)
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
					static bool translate = false;
					static bool rotate = false;
					static bool scale = false;
					if (this->CurrentOperation == ImGuizmo::TRANSLATE)
					{
						translate = true;
						rotate = false;
						scale = false;
					}
					else if (this->CurrentOperation == ImGuizmo::ROTATE)
					{
						rotate = true;
						translate = false;
						scale = false;
					}
					else if (this->CurrentOperation == ImGuizmo::SCALE)
					{
						scale = true;
						rotate = false;
						translate = false;
					}
					if (ImGui::Checkbox("Translation", &translate))
					{
						this->CurrentOperation = ImGuizmo::TRANSLATE;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Rotation", &rotate))
					{
						this->CurrentOperation = ImGuizmo::ROTATE;
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Scaling", &scale))
					{
						this->CurrentOperation = ImGuizmo::SCALE;
					}

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

					float scaling[3] = { transform.Scale.x, transform.Scale.y, transform.Scale.z };
					if (ImGui::DragFloat3("Scale", scaling, 0.1f, 0.0f))
					{
						transform.Scale = GVector3(scaling[0], scaling[1], scaling[2]);
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

					if (IsScriptExists)
					{
						if (this->PanelScene->GetSceneState() == GE_SCENE_RUNTIME)
						{
							auto entityClass = GuardianScriptEngine::GetEntityClass(script.ClassName);
							for (const auto& field : entityClass->GetClassFieldList())
							{
								if (field.second.Type == GuardianScriptClass::ScriptField::GE_FLOAT)
								{
									float data = *(float*)entityClass->GetFieldValue(field.first);
									if (ImGui::DragFloat(field.first.c_str(), &data, 0.1f))
									{
										entityClass->SetFieldValue(field.first, (void*)&data);
									}
								}
								else if (field.second.Type == GuardianScriptClass::ScriptField::GE_INT)
								{
									int data = *(int*)entityClass->GetFieldValue(field.first);
									if (ImGui::DragInt(field.first.c_str(), &data))
									{
										entityClass->SetFieldValue(field.first, (void*)&data);
									}
								}
							}
						}
						else if (this->PanelScene->GetSceneState() == GE_SCENE_EDIT)
						{
							auto entityClass = GuardianScriptEngine::GetEntityClass(script.ClassName);
							for (auto& field : entityClass->GetClassFieldList())
							{
								if (field.second.Type == GuardianScriptClass::ScriptField::GE_FLOAT)
								{
									float data = field.second.GetValue<float>();
									if (ImGui::DragFloat(field.first.c_str(), &data, 0.1f))
									{
										field.second.SetValue(data);
									}
								}
								else if (field.second.Type == GuardianScriptClass::ScriptField::GE_INT)
								{
									int data = field.second.GetValue<int>();
									if (ImGui::DragInt(field.first.c_str(), &data))
									{
										field.second.SetValue(data);
									}
								}
							}
						}
					}

					if (!IsScriptExists)
					{
						ImGui::PopStyleColor();
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianPointLightComponent>())
			{
				auto& PointLight = SelectedEntity->GetComponent<GuardianPointLightComponent>().LightProperties;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianPointLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Point Light Component"))
				{
					float color[3] = { PointLight.LightColor.x, PointLight.LightColor.y, PointLight.LightColor.z };
					if (ImGui::DragFloat3("Light Color", color, 0.01f, 0.0f, 1.0f))
					{
						PointLight.LightColor = GVector3(color[0], color[1], color[2]);
					}

					float strength = PointLight.LightStrength;
					if (ImGui::DragFloat("Light Strength", &strength, 1.0f))
					{
						PointLight.LightStrength = strength;
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianMeshComponent>())
			{
				auto& mesh = SelectedEntity->GetComponent<GuardianMeshComponent>().Mesh;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianMeshComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Mesh Component"))
				{
					ImGui::Text("Mesh Filter");
					ImGui::SameLine();
					ImGui::Button((mesh->GetMeshName() + "##Mesh").c_str());

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCES_BROWSER_ITEM"))
						{
							GString Path = (const char*)payload->Data;
							GString FileType = std::filesystem::path(GString((const char*)payload->Data)).extension().string();
							if (FileType == ".obj" || FileType == ".fbx")
							{
								mesh.reset();
								mesh = std::make_shared<GuardianMesh>();
								auto& meshAsset = GuardianAssetSystem::GetMeshAssetFromPath(Path);
								mesh->InitializeMesh(meshAsset.GetAssetName(), meshAsset.GetMeshAssetData());
							}
						}
						
						ImGui::EndDragDropTarget();
					}

					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						openMeshBrowser = true;
					}

					if (ImGui::TreeNodeEx((void*)typeid(GuardianMaterial).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
						"Mesh Materials"))
					{
						for (int i = 0; i < (int)mesh->MeshInstancesList.size(); i++)
						{
							ImGui::Text(("Element " + std::to_string(i)).c_str());

							ImGui::SameLine();

							ImGui::Image((ImTextureID)GuardianMaterialSystem::GetMaterialRenderingView(
								mesh->MeshInstancesList[i]->GetMeshInstanceData().MaterialId)->GetFramebufferResource().Get(),
								ImVec2(90.0f, 90.0f), { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.8f, 0.8f, 0.8f, 1.0f });
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianSphereColliderComponent>())
			{
				auto& collider = SelectedEntity->GetComponent<GuardianSphereColliderComponent>().SphereCollider;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianSphereColliderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Sphere Collider Component"))
				{
					float radius = collider->GetColliderProperties().Radius;
					if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.0f))
					{
						collider->SetColliderProperties({ radius });
					}

					ImGui::Separator();

					ImGui::Text("Physics Material");

					float staticFriction = collider->GetColliderMaterial().GetStaticFriction();
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							collider->GetColliderMaterial().GetDynamicFriction(),
							restitution));
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianBoxColliderComponent>())
			{
				auto& collider = SelectedEntity->GetComponent<GuardianBoxColliderComponent>().BoxCollider;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianBoxColliderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Box Collider Component"))
				{
					float size[3] = { collider->GetColliderProperties().BoxHalfsize.x * 2.0f,
						collider->GetColliderProperties().BoxHalfsize.y * 2.0f,
						collider->GetColliderProperties().BoxHalfsize.z * 2.0f };
					if (ImGui::DragFloat3("Size", size, 0.1f, 0.0f))
					{
						GVector3 Halfsize = { size[0] / 2.0f, size[1] / 2.0f, size[2] / 2.0f };
						collider->SetColliderProperties({ Halfsize });
					}

					ImGui::Separator();

					ImGui::Text("Physics Material");

					float staticFriction = collider->GetColliderMaterial().GetStaticFriction();
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							collider->GetColliderMaterial().GetDynamicFriction(),
							restitution));
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianCapsuleColliderComponent>())
			{
				auto& collider = SelectedEntity->GetComponent<GuardianCapsuleColliderComponent>().CapsuleCollider;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianCapsuleColliderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Capsule Collider Component"))
				{
					float radius = collider->GetColliderProperties().Radius;
					if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.0f))
					{
						collider->SetColliderProperties({ radius, collider->GetColliderProperties().HalfHeight });
					}

					float height = collider->GetColliderProperties().HalfHeight * 2.0f;
					if (ImGui::DragFloat("Height", &height, 0.1f, 0.0f))
					{
						collider->SetColliderProperties({ collider->GetColliderProperties().Radius, height / 2.0f });
					}

					ImGui::Separator();

					ImGui::Text("Physics Material");

					float staticFriction = collider->GetColliderMaterial().GetStaticFriction();
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							collider->GetColliderMaterial().GetDynamicFriction(),
							restitution));
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianMeshColliderComponent>())
			{
				auto& collider = SelectedEntity->GetComponent<GuardianMeshColliderComponent>().MeshCollider;

				if (ImGui::TreeNodeEx((void*)typeid(GuardianMeshColliderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Mesh Collider Component"))
				{
					ImGui::Text("Mesh Filter : ");
					ImGui::SameLine();
					ImGui::Text("Use Mesh Component");

					ImGui::Separator();

					ImGui::Text("Physics Material");

					float staticFriction = collider->GetColliderMaterial().GetStaticFriction();
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.00001f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							collider->GetColliderMaterial().GetDynamicFriction(),
							restitution));
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianRigidBodyComponent>())
			{
				auto& component = SelectedEntity->GetComponent<GuardianRigidBodyComponent>();

				if (ImGui::TreeNodeEx((void*)typeid(GuardianRigidBodyComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Rigid Body Component"))
				{
					static const char* types[] = { "Static", "Dynamic" };
					int index = 0;
					if (component.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						index = 0;
					}
					else if (component.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						index = 1;
					}
					if (ImGui::BeginCombo("Type", types[index]))
					{
						if (ImGui::Selectable("Static", true))
						{
							component.RigidBodyType = GE_RIGIDBODY_STATIC;
						}
						if (ImGui::Selectable("Dynamic"))
						{
							component.RigidBodyType = GE_RIGIDBODY_DYNAMIC;
						}

						ImGui::EndCombo();
					}

					if (component.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						float density = component.DynamicRigidBody->GetRigidBodyDensity();
						if (ImGui::DragFloat("Density", &density, 0.1f, 0.0f))
						{
							component.DynamicRigidBody->SetRigidBodyDensity(density);
						}
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianTransformComponent>())
				{
					if (ImGui::MenuItem("Transform Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianTransformComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianCameraComponent>())
				{
					if (ImGui::MenuItem("Camera Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianCameraComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianScriptComponent>())
				{
					if (ImGui::MenuItem("Script Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianScriptComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianPointLightComponent>())
				{
					if (ImGui::MenuItem("Point Light Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianPointLightComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianMeshComponent>())
				{
					if (ImGui::MenuItem("Mesh Component"))
					{
						auto& meshData = GuardianAssetSystem::GetMeshAssetList()["Box"]->GetMeshAssetData();
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianMeshComponent>().Mesh = std::make_shared<GuardianMesh>();
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							GetComponent<GuardianMeshComponent>().Mesh->InitializeMesh("Box", meshData);
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianSphereColliderComponent>())
				{
					if (ImGui::MenuItem("Sphere Collider Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianSphereColliderComponent>().SphereCollider->SetColliderMaterial(
								GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianBoxColliderComponent>())
				{
					if (ImGui::MenuItem("Box Collider Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianBoxColliderComponent>().BoxCollider->SetColliderMaterial(
								GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianCapsuleColliderComponent>())
				{
					if (ImGui::MenuItem("Capsule Collider Component"))
					{
						this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianCapsuleColliderComponent>().CapsuleCollider->SetColliderMaterial(
								GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
						ImGui::CloseCurrentPopup();
					}
				}

				if (!this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
					HasComponent<GuardianRigidBodyComponent>())
				{
					if (ImGui::MenuItem("RigidBody Component"))
					{
						auto& type = this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
							AddComponent<GuardianRigidBodyComponent>().RigidBodyType;
						if (type == GE_RIGIDBODY_STATIC)
						{
							this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
								GetComponent<GuardianRigidBodyComponent>().StaticRigidBody->SetRigidBodyTransform(
									SelectedEntity->GetComponent<GuardianTransformComponent>());
						}
						else if (type == GE_RIGIDBODY_DYNAMIC)
						{
							this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
								GetComponent<GuardianRigidBodyComponent>().DynamicRigidBody->SetRigidBodyTransform(
									SelectedEntity->GetComponent<GuardianTransformComponent>());
						}
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}
	}

	void GuardianSceneHierarchyPanel::RenderMeshBrowser(GString& meshName, bool& open)
	{
		if (open)
		{
			ImGui::Begin("Select a Mesh", &open);

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

			for (auto& meshPair : GuardianAssetSystem::GetMeshAssetList())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

				ImGui::ImageButton(meshPair.first.c_str(),
					(ImTextureID)GuardianAssetSystem::GetMeshAssetRenderingView(meshPair.first)->GetFramebufferResource().Get(), ImVec2(IconSize, IconSize));
				ImGui::PopStyleColor(3);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					meshName = meshPair.first;

					open = false;
				}
				ImGui::TextWrapped(meshPair.first.c_str());

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			ImGui::End();
		}
	}
}
