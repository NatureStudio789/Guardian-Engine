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
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->MeshFileIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/MeshFileIcon.png"));
	}

	GuardianSceneHierarchyPanel::GuardianSceneHierarchyPanel(GuardianScene* scene)
	{
		this->PanelName = "Scene Hierarchy";
		this->SelectedEntityId = 0;
		this->CurrentOperation = (int)ImGuizmo::TRANSLATE;
		this->MeshFileIcon = GuardianTexture(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("../Guardian Engine/Resources/Textures/MeshFileIcon.png"));
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
		static bool OpenMeshBrowser = false;
		static bool OpenMaterialBrowser = false;
		static bool OpenTextureBrowser = false;
		int SelectedTextureIndex = -1;
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
			GuardianMesh mesh = GuardianResourceSystem::GetMesh(meshName);
			this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().MeshName = meshName;
			this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().Mesh = std::make_shared<GuardianMesh>(mesh);
		}

		GString materialName;
		this->RenderMaterialBrowser(materialName, OpenMaterialBrowser);
		if (!materialName.empty())
		{
			GuardianMaterial material = GuardianResourceSystem::GetMaterial(materialName);
			this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
				Mesh->SetMeshMaterial(std::make_shared<GuardianMaterial>(material));
		}

		GString textureName;
		this->RenderTextureBrowser(textureName, OpenTextureBrowser);
		if (!textureName.empty())
		{
			GuardianTexture texture = GuardianResourceSystem::GetTexture(textureName);
			if (SelectedTextureIndex == 0)
			{
				this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
					Mesh->GetMaterial()->SetAlbedoTexture(std::make_shared<GuardianTexture>(texture));
			}
			else if (SelectedTextureIndex == 1)
			{
				this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
					Mesh->GetMaterial()->SetMetallicTexture(std::make_shared<GuardianTexture>(texture));
			}
			else if (SelectedTextureIndex == 2)
			{
				this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
					Mesh->GetMaterial()->SetRoughnessTexture(std::make_shared<GuardianTexture>(texture));
			}
			else if (SelectedTextureIndex == 3)
			{
				this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
					Mesh->GetMaterial()->SetNormalTexture(std::make_shared<GuardianTexture>(texture));
			}
			else if (SelectedTextureIndex == 4)
			{
				this->PanelScene->GetEntity(this->SelectedEntityId)->GetComponent<GuardianMeshComponent>().
					Mesh->GetMaterial()->SetAoTexture(std::make_shared<GuardianTexture>(texture));
			}
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
					if (ImGui::Button((SelectedEntity->GetComponent<GuardianMeshComponent>().MeshName + "##Mesh").c_str()))
					{
						openMeshBrowser = true;
					}

					ImGui::Text("Mesh Material");
					ImGui::SameLine();
					if (ImGui::Button((GuardianResourceSystem::GetMaterialName(
						*SelectedEntity->GetComponent<GuardianMeshComponent>().Mesh->GetMaterial()) + "##Material").c_str()))
					{
						openMaterialBrowser = true;
					}

					ImGui::Separator();

					if (ImGui::TreeNodeEx((void*)typeid(GuardianMaterial).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
						GuardianResourceSystem::GetMaterialName(
							*SelectedEntity->GetComponent<GuardianMeshComponent>().Mesh->GetMaterial()).c_str()))
					{
						auto material = SelectedEntity->GetComponent<GuardianMeshComponent>().Mesh->GetMaterial();

						if (ImGui::TreeNodeEx("Albedo Color (Diffuse Color)", ImGuiTreeNodeFlags_DefaultOpen))
						{
							static bool UseAlbedoT = material->UsingAlbedoTexture;
							ImGui::Checkbox("Use Texture", &UseAlbedoT);

							if (!UseAlbedoT)
							{
								float albedoColor[3] = { material->AlbedoColor.x, material->AlbedoColor.y, material->AlbedoColor.z };
								if (ImGui::ColorEdit3("##albedo color", albedoColor))
								{
									material->SetAlbedoColor(GVector3(albedoColor[0], albedoColor[1], albedoColor[2]));
								}
							}
							else
							{
								if (!material->AlbedoTexture)
								{
									openTextureBrowser = true;
									textureIndex = 0;
								}
								else
								{
									if (ImGui::ImageButton("Albedo",
										(ImTextureID)material->AlbedoTexture->GetTextureResource().Get(), ImVec2(50.0f, 50.0f)))
									{
										openTextureBrowser = true;
										textureIndex = 0;
									}
								}
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Metallic Color", ImGuiTreeNodeFlags_DefaultOpen))
						{
							static bool UseMetallicT = material->UsingMetallicTexture;
							ImGui::Checkbox("Use Texture", &UseMetallicT);

							if (!UseMetallicT)
							{
								float metallicColor = material->MetallicColor;
								if (ImGui::DragFloat("##metallic color", &metallicColor, 0.001f, 0.0f, 1.0f))
								{
									material->SetMetallicColor(metallicColor);
								}
							}
							else
							{
								if (!material->MetallicTexture)
								{
									openTextureBrowser = true;
									textureIndex = 1;
								}
								else
								{
									if (ImGui::ImageButton("Metallic",
										(ImTextureID)material->MetallicTexture->GetTextureResource().Get(), ImVec2(50.0f, 50.0f)))
									{
										openTextureBrowser = true;
										textureIndex = 1;
									}
								}
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Roughness Color", ImGuiTreeNodeFlags_DefaultOpen))
						{
							static bool UseRoughnessT = material->UsingRoughnessTexture;
							ImGui::Checkbox("Use Texture", &UseRoughnessT);

							if (!UseRoughnessT)
							{
								float roughnessColor = material->RoughnessColor;
								if (ImGui::DragFloat("##roughness color", &roughnessColor, 0.001f, 0.0f, 1.0f))
								{
									material->SetRoughnessColor(roughnessColor);
								}
							}
							else
							{
								if (!material->RoughnessTexture)
								{
									openTextureBrowser = true;
									textureIndex = 2;
								}
								else
								{
									if (ImGui::ImageButton("Roughness",
										(ImTextureID)material->RoughnessTexture->GetTextureResource().Get(), ImVec2(50.0f, 50.0f)))
									{
										openTextureBrowser = true;
										textureIndex = 2;
									}
								}
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Normal", ImGuiTreeNodeFlags_DefaultOpen))
						{
							static bool UseNormalT = material->UsingNormalTexture;
							ImGui::Checkbox("Use Texture", &UseNormalT);

							if (!UseNormalT)
							{
								ImGui::Button("Default Normal");
							}
							else
							{
								if (!material->NormalTexture)
								{
									openTextureBrowser = true;
									textureIndex = 3;
								}
								else
								{
									if (ImGui::ImageButton("Normal",
										(ImTextureID)material->NormalTexture->GetTextureResource().Get(), ImVec2(50.0f, 50.0f)))
									{
										openTextureBrowser = true;
										textureIndex = 3;
									}
								}
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Ao Color", ImGuiTreeNodeFlags_DefaultOpen))
						{
							static bool UseAoT = material->UsingAoTexture;
							ImGui::Checkbox("Use Texture", &UseAoT);

							if (!UseAoT)
							{
								float aoColor = material->AoColor;
								if (ImGui::DragFloat("##ao color", &aoColor, 0.001f, 0.0f, 1.0f))
								{
									material->SetAoColor(aoColor);
								}
							}
							else
							{
								if (!material->AoTexture)
								{
									openTextureBrowser = true;
									textureIndex = 4;
								}
								else
								{
									if (ImGui::ImageButton("Ao",
										(ImTextureID)material->AoTexture->GetTextureResource().Get(), ImVec2(50.0f, 50.0f)))
									{
										openTextureBrowser = true;
										textureIndex = 4;
									}
								}
							}

							ImGui::TreePop();
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}

				ImGui::Separator();
			}

			if (SelectedEntity->HasComponent<GuardianModelComponent>())
			{
				auto& model = SelectedEntity->GetComponent<GuardianModelComponent>();

				if (ImGui::TreeNodeEx((void*)typeid(GuardianModelComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Model Component"))
				{
					GString fileName = model.GetModelFilePath().substr(model.GetModelFilePath().find_last_of('/') + 1);
					if (fileName == model.GetModelFilePath())
					{
						fileName = model.GetModelFilePath().substr(model.GetModelFilePath().find_last_of('\\') + 1);
					}
					ImGui::Text("Model File Path");
					ImGui::SameLine();
					if (ImGui::Button(fileName.c_str()))
					{
						GString filePath = GuardianFileDialog::OpenFile("Model File (*.obj)\0*.obj\0");
						if (std::filesystem::exists(filePath) && std::filesystem::path(filePath).extension() == ".obj")
						{
							model.ClearModelMeshList();
							model.InitializeModel(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
								filePath);
						}
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
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f))
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
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f))
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
					if (ImGui::DragFloat("Static Friction", &staticFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(staticFriction,
							collider->GetColliderMaterial().GetDynamicFriction(),
							collider->GetColliderMaterial().GetRestitution()));
					}

					float dynamicFriction = collider->GetColliderMaterial().GetDynamicFriction();
					if (ImGui::DragFloat("Dynamic Friction", &dynamicFriction, 0.01f, 0.0f))
					{
						collider->SetColliderMaterial(GuardianPhysicsMaterial(
							collider->GetColliderMaterial().GetStaticFriction(),
							dynamicFriction,
							collider->GetColliderMaterial().GetRestitution()));
					}

					float restitution = collider->GetColliderMaterial().GetRestitution();
					if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f))
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

				if (ImGui::MenuItem("Point Light Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianPointLightComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Mesh Component"))
				{
					GuardianMesh mesh = GuardianResourceSystem::GetMeshList()["Box"];
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianMeshComponent>().Mesh = std::make_shared<GuardianMesh>(mesh);
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						GetComponent<GuardianMeshComponent>().MeshName = "Box";
				}

				if (ImGui::MenuItem("Model Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianModelComponent>().InitializeModel(
							GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), 
							"Resources/Models/SBDS/SBDS.obj");
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sphere Collider Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianSphereColliderComponent>().SphereCollider->SetColliderMaterial(
							GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Box Collider Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianBoxColliderComponent>().BoxCollider->SetColliderMaterial(
							GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Capsule Collider Component"))
				{
					this->PanelScene->SceneEntityList[SelectedEntity->GetEntityHandle()]->
						AddComponent<GuardianCapsuleColliderComponent>().CapsuleCollider->SetColliderMaterial(
							GuardianPhysicsMaterial(0.5f, 0.5f, 0.6f));
					ImGui::CloseCurrentPopup();
				}

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

			for (auto& meshPair : GuardianResourceSystem::GetMeshList())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

				ImGui::ImageButton(meshPair.first.c_str(),
					(ImTextureID)this->MeshFileIcon.GetTextureResource().Get(), ImVec2(IconSize, IconSize));
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

	void GuardianSceneHierarchyPanel::RenderTextureBrowser(GString& textureName, bool& open)
	{
		if (open)
		{
			ImGui::Begin("Select a Texture", &open);

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

			for (auto& texturePair : GuardianResourceSystem::GetTextureList())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

				ImGui::ImageButton(texturePair.first.c_str(),
					(ImTextureID)texturePair.second.GetTextureResource().Get(), ImVec2(IconSize, IconSize));
				ImGui::PopStyleColor(3);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					textureName = texturePair.first;

					open = false;
				}
				ImGui::TextWrapped(texturePair.first.c_str());

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			ImGui::End();
		}
	}

	void GuardianSceneHierarchyPanel::RenderMaterialBrowser(GString& materialName, bool& open)
	{
		if (open)
		{
			ImGui::Begin("Select a Material", &open);

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

			for (auto& materialPair : GuardianResourceSystem::GetMaterialList())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

				ImGui::ImageButton(materialPair.first.c_str(),
					(ImTextureID)this->MeshFileIcon.GetTextureResource().Get(), ImVec2(IconSize, IconSize));
				ImGui::PopStyleColor(3);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					materialName = materialPair.first;

					open = false;
				}
				ImGui::TextWrapped(materialPair.first.c_str());

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			ImGui::End();
		}
	}
}
