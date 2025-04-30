#include "GuardianScene.h"
#include "../Entity/GuardianEntity.h"
#include "../Entity/GuardianScriptableEntity.h"
#include "../../Application/GuardianApplication.h"

namespace GE
{
	GuardianScene::GuardianScene()
	{
		this->SceneName = "Unnamed";
		this->SceneEntityList.clear();
		this->EditorCamera = std::make_shared<GuardianCamera>(GVector3(0.0f, 8.0f, -30.0f), 
			GVector3(15.0f, 0.0f, 0.0f), GuardianPerspectiveProjection(60.0f, 16.0f / 9.0f, 0.001f, 10000.0f));
		this->RuntimeCamera = std::make_shared<GuardianCamera>();
		this->ShouldOperateCamera = false;
		this->SceneGrid = null;
		this->SceneState = GE_SCENE_EDIT;

		this->PhysicsWorld = null;
		this->SceneGravity = GE_GRAVITY_EARTH;
	}

	GuardianScene::GuardianScene(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->EditorCamera = std::make_shared<GuardianCamera>(GVector3(0.0f, 8.0f, -30.0f),
			GVector3(0.0f, 0.0f, 0.0f), GuardianPerspectiveProjection(60.0f, 16.0f / 9.0f, 0.001f, 10000.0f));
		this->ShouldOperateCamera = false;
		this->SceneGrid = null;
		this->RuntimeCamera = std::make_shared<GuardianCamera>();
		this->SceneState = GE_SCENE_EDIT;

		this->PhysicsWorld = null;
		this->SceneGravity = GE_GRAVITY_EARTH;

		this->LoadScene(graphics);
	}

	GuardianScene::GuardianScene(const GuardianScene& other)
	{
		this->SceneState = other.SceneState;
		this->SceneEntityList = other.SceneEntityList;
		this->EditorCamera = other.EditorCamera;
		this->RuntimeCamera = other.RuntimeCamera;
		this->ShouldOperateCamera = other.ShouldOperateCamera;
		this->SceneGrid = other.SceneGrid;

		this->PhysicsWorld = other.PhysicsWorld;
		this->SceneGravity = other.SceneGravity;
	}

	void GuardianScene::InitializeScene()
	{
		GuardianRenderer::CreateRenderingRenderGraph(this->SceneName + " Scene Rendering",
			GuardianApplication::ApplicationInstance->GetApplicationWindow()->GetWindowProperties().GetWidth(),
			GuardianApplication::ApplicationInstance->GetApplicationWindow()->GetWindowProperties().GetHeight());

		GuardianRenderer::SetRenderingRenderGraphCamera(this->SceneName + " Scene Rendering", *this->EditorCamera);

		GuardianRenderer::SetRenderingRenderGraphShaderGroup(this->SceneName + " Scene Rendering", GuardianShaderSystem::PBR_MAIN_SHADER);

		GuardianRenderer::SetRenderingRenderGraphClearColor(this->SceneName + " Scene Rendering", GVector3(0.1f, 0.1f, 0.1f));
	}

	void GuardianScene::SetSceneName(const GString& name)
	{
		this->SceneName = name;
	}

	void GuardianScene::LoadScene(std::shared_ptr<GuardianGraphics> graphics)
	{
		if (this->SceneState == GE_SCENE_RUNTIME)
		{
			this->StopRuntime();
		}

		this->RemoveAllEntity();

		GString sceneFilePath = GuardianFileDialog::OpenFile("Guardian Engine Scene (*.gscene)\0*.gscene\0");

		this->Deserialize(sceneFilePath);
	}

	void GuardianScene::LoadSceneAs(std::shared_ptr<GuardianGraphics> graphics, const GString& sceneFilePath)
	{
		if (this->SceneState == GE_SCENE_RUNTIME)
		{
			this->StopRuntime();
		}

		this->RemoveAllEntity();

		this->Deserialize(sceneFilePath);
	}

	void GuardianScene::SaveScene()
	{
		if (!this->IsFileExists())
		{
			this->InitializeFile(GuardianFileDialog::SaveFile("Guardian Engine Scene (*.gscene)\0*.gscene\0"));
		}

		this->Serialize();
	}

	void GuardianScene::SaveSceneAs(const GString& filePath)
	{
		this->InitializeFile(filePath);

		this->Serialize();
	}

	void GuardianScene::UpdateScene(GuardianTimestep deltaTime)
	{
		switch (this->SceneState)
		{
			case GE_SCENE_EDIT:
			{
				this->UpdateEditScene(deltaTime);
				break;
			}

			case GE_SCENE_RUNTIME:
			{
				this->UpdateRuntimeScene(deltaTime);
				break;
			}
		}
	}

	void GuardianScene::UpdateEditScene(GuardianTimestep deltaTime)
	{
		GuardianRenderer::SetRenderingRenderGraphCamera(this->SceneName + " Scene Rendering", *this->EditorCamera);
		GuardianRenderer::SetRenderingRenderGraphClearColor(this->SceneName + " Scene Rendering", GVector3(0.1f, 0.1f, 0.1f));

		if (this->ShouldOperateCamera)
		{
			if (GuardianKeyboard::IsKeyPress('W'))
			{
				this->EditorCamera->Translate(this->EditorCamera->GetForwardVector() * 0.1f);
			}
			if (GuardianKeyboard::IsKeyPress('S'))
			{
				this->EditorCamera->Translate(this->EditorCamera->GetBackwardVector() * 0.1f);
			}
			if (GuardianKeyboard::IsKeyPress('A'))
			{
				this->EditorCamera->Translate(this->EditorCamera->GetLeftVector() * 0.1f);
			}
			if (GuardianKeyboard::IsKeyPress('D'))
			{
				this->EditorCamera->Translate(this->EditorCamera->GetRightVector() * 0.1f);
			}
			if (GuardianKeyboard::IsKeyPress(VK_SPACE))
			{
				this->EditorCamera->Translate(GVector3(0.0f, 0.1f, 0.0f));
			}
			if (GuardianKeyboard::IsKeyPress(VK_SHIFT))
			{
				this->EditorCamera->Translate(GVector3(0.0f, -0.1f, 0.0f));
			}

			if (GuardianInput::IsMouseButtonClick(GE_MOUSEBUTTON_RIGHT))
			{
				while (!GuardianMouse::IsMouseRawMoveEventListEmpty())
				{
					GuardianMouseRawMoveEvent event = GuardianMouse::ReadRawMoveEvent();
					this->EditorCamera->Rotate(GVector3(event.MouseMovementY * 0.1f, event.MouseMovementX * 0.1f, 0.0f));
				}
			}
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianPointLightComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianPointLightComponent& PLComponent)
				{
					PLComponent.LightProperties.LightPosition = TComponent.Position;

					if (!PLComponent.LightMesh)
					{
						PLComponent.LightMesh = std::make_shared<GuardianMesh>();
						PLComponent.LightMesh->InitializeMesh("Light", "../Guardian Engine/Assets/Models/Light/Light.obj");
					}
					PLComponent.LightMesh->SubmitToRenderer(this->SceneName + " Scene Rendering");

					GuardianLightSystem::SubmitPointLight(PLComponent.LightProperties);
					GuardianTransform transform;
					transform.Position = TComponent.Position;
					transform.Rotation = TComponent.Rotation;
					transform.Quaternion = TComponent.Quaternion;
					transform.Scale = GVector3(1.0f, 1.0f, 1.0f);
					PLComponent.LightMesh->UpdateMeshTransform(transform.GetTransformMatrix());
				});
		}
		GuardianLightProperties LightProperties;
		int index = 0;
		while (!GuardianLightSystem::IsPointLightListEmpty())
		{
			GuardianPointLightProperties pointLight = GuardianLightSystem::ReadPointLight();
			LightProperties.PointLightList[index] = pointLight;
			index++;
		}
		LightProperties.LightNumber = index;
		LightProperties.CameraPosition = this->EditorCamera->Position;

		if (!this->SceneGrid)
		{
			this->SceneGrid = std::make_shared<GuardianMesh>();
			this->SceneGrid->InitializeMesh("Grid", "../Guardian Engine/Assets/Models/Grid/Grid.obj");
		}
		this->SceneGrid->SubmitToRenderer(this->SceneName + " Scene Rendering");
		this->SceneGrid->UpdateMeshTransform(GuardianTransform().GetTransformMatrix());
		this->SceneGrid->UpdateMeshLighting(LightProperties);

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianSphereColliderComponent>();
			view.each([this](const auto& e, GuardianTransformComponent& TComponent,
				GuardianSphereColliderComponent& SCComponent)
			{
				if (!SCComponent.SphereGeometry->IsInitialized())
				{
					SCComponent.SphereGeometry->InitializeGeometry(GE_GEOMETRY_SPHERE);
				}

				GuardianTransform transform = GuardianTransform(TComponent.Position, TComponent.Rotation,
					TComponent.Quaternion, GVector3(1.0f, 1.0f, 1.0f) * SCComponent.SphereCollider->GetColliderProperties().Radius);
				SCComponent.SphereGeometry->UpdateGeometryTransform(transform.GetTransformMatrix());

				GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->SceneName + " Scene Rendering", SCComponent.SphereGeometry);
			});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianBoxColliderComponent>();
			view.each([this](const auto& e, GuardianTransformComponent& TComponent,
				GuardianBoxColliderComponent& BCComponent)
				{
					if (!BCComponent.BoxGeometry->IsInitialized())
					{
						BCComponent.BoxGeometry->InitializeGeometry(GE_GEOMETRY_BOX);
					}

					GuardianTransform transform = GuardianTransform(TComponent.Position, TComponent.Rotation,
						TComponent.Quaternion, BCComponent.BoxCollider->GetColliderProperties().BoxHalfsize * 2.0f);
					BCComponent.BoxGeometry->UpdateGeometryTransform(transform.GetTransformMatrix());

					GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->SceneName + " Scene Rendering", BCComponent.BoxGeometry);
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianCapsuleColliderComponent>();
			view.each([this](const auto& e, GuardianTransformComponent& TComponent,
				GuardianCapsuleColliderComponent& CCComponent)
				{
					if (!CCComponent.CapsuleGeometry->IsInitialized())
					{
						CCComponent.CapsuleGeometry->InitializeGeometry(GE_GEOMETRY_CAPSULE);
					}

					GuardianTransform transform = GuardianTransform(TComponent.Position, TComponent.Rotation,
						TComponent.Quaternion, GVector3(CCComponent.CapsuleCollider->GetColliderProperties().Radius, 
							CCComponent.CapsuleCollider->GetColliderProperties().HalfHeight * 2.0f, 
							CCComponent.CapsuleCollider->GetColliderProperties().Radius));
					CCComponent.CapsuleGeometry->UpdateGeometryTransform(transform.GetTransformMatrix());

					GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->SceneName + " Scene Rendering", CCComponent.CapsuleGeometry);
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianRigidBodyComponent& RBComponent)
			{
				if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
				{
					RBComponent.StaticRigidBody->SetRigidBodyTransform(TComponent);
				}
				else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
				{
					RBComponent.DynamicRigidBody->SetRigidBodyTransform(TComponent);
				}
			});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianCameraComponent>();
			view.each([this, &LightProperties](const auto& e, 
				GuardianTransformComponent& TComponent, GuardianCameraComponent& CComponent)
			{
				CComponent.Position = TComponent.Position;
				CComponent.Direction = TComponent.Rotation;

				if (!CComponent.CameraMesh)
				{
					CComponent.CameraMesh = std::make_shared<GuardianMesh>();
					CComponent.CameraMesh->InitializeMesh("Camera", "../Guardian Engine/Assets/Models/Camera/Camera.obj");
				}
				CComponent.CameraMesh->SubmitToRenderer(this->SceneName + " Scene Rendering");

				GuardianTransform transform;
				transform.Position = TComponent.Position;
				transform.Rotation = TComponent.Rotation;
				transform.Quaternion = TComponent.Quaternion;
				transform.Scale = GVector3(1.0f, 1.0f, 1.0f);
				CComponent.CameraMesh->UpdateMeshTransform(transform.GetTransformMatrix());
				CComponent.CameraMesh->UpdateMeshLighting(LightProperties);
			});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianMeshComponent>();
			view.each([this, &LightProperties](const auto& e,
				GuardianTransformComponent& TComponent, GuardianMeshComponent& MComponent)
				{
					MComponent.Mesh->UpdateMeshTransform(TComponent.GetTransformMatrix());
					MComponent.Mesh->UpdateMeshLighting(LightProperties);

					MComponent.Mesh->SubmitToRenderer(this->SceneName + " Scene Rendering");
				});
		}
	}

	void GuardianScene::StartRuntime()
	{
		this->SceneState = GE_SCENE_RUNTIME;

		this->CreatePhysicsWorld();
		this->Serialize("Temp.gdata", false);

		{
			auto view = this->SceneRegistry.view<GuardianSphereColliderComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e, GuardianSphereColliderComponent& SCComponet,
				GuardianRigidBodyComponent& RBComponent)
				{
					if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						SCComponet.SphereCollider->InitializeSphereCollider();

						if (!RBComponent.StaticRigidBody->GetRigidBodyCollider())
						{
							RBComponent.StaticRigidBody->SetRigidBodyCollider(SCComponet.SphereCollider);
						}

						RBComponent.StaticRigidBody->InitializeStaticRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.StaticRigidBody->GetRigidBodyObject());
					}
					else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						SCComponet.SphereCollider->InitializeSphereCollider();

						if (!RBComponent.DynamicRigidBody->GetRigidBodyCollider())
						{
							RBComponent.DynamicRigidBody->SetRigidBodyCollider(SCComponet.SphereCollider);
						}

						RBComponent.DynamicRigidBody->InitializeDynamicRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.DynamicRigidBody->GetRigidBodyObject());
					}
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianBoxColliderComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e, GuardianBoxColliderComponent& BCComponet,
				GuardianRigidBodyComponent& RBComponent)
				{
					if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						BCComponet.BoxCollider->InitializeBoxCollider();

						if (!RBComponent.StaticRigidBody->GetRigidBodyCollider())
						{
							RBComponent.StaticRigidBody->SetRigidBodyCollider(BCComponet.BoxCollider);
						}

						RBComponent.StaticRigidBody->InitializeStaticRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.StaticRigidBody->GetRigidBodyObject());
					}
					else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						BCComponet.BoxCollider->InitializeBoxCollider();

						if (!RBComponent.DynamicRigidBody->GetRigidBodyCollider())
						{
							RBComponent.DynamicRigidBody->SetRigidBodyCollider(BCComponet.BoxCollider);
						}

						RBComponent.DynamicRigidBody->InitializeDynamicRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.DynamicRigidBody->GetRigidBodyObject());
					}
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianCapsuleColliderComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e, GuardianCapsuleColliderComponent& CCComponet,
				GuardianRigidBodyComponent& RBComponent)
				{
					if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						CCComponet.CapsuleCollider->InitializeCapsuleCollider();

						if (!RBComponent.StaticRigidBody->GetRigidBodyCollider())
						{
							RBComponent.StaticRigidBody->SetRigidBodyCollider(CCComponet.CapsuleCollider);
						}

						RBComponent.StaticRigidBody->InitializeStaticRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.StaticRigidBody->GetRigidBodyObject());
					}
					else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						CCComponet.CapsuleCollider->InitializeCapsuleCollider();

						if (!RBComponent.DynamicRigidBody->GetRigidBodyCollider())
						{
							RBComponent.DynamicRigidBody->SetRigidBodyCollider(CCComponet.CapsuleCollider);
						}

						RBComponent.DynamicRigidBody->InitializeDynamicRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.DynamicRigidBody->GetRigidBodyObject());
					}
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianMeshColliderComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e, GuardianMeshColliderComponent& MCComponet,
				GuardianRigidBodyComponent& RBComponent)
				{
					if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						MCComponet.MeshCollider->InitializeMeshCollider();

						if (!RBComponent.StaticRigidBody->GetRigidBodyCollider())
						{
							RBComponent.StaticRigidBody->SetRigidBodyCollider(MCComponet.MeshCollider);
						}

						RBComponent.StaticRigidBody->InitializeStaticRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.StaticRigidBody->GetRigidBodyObject());
					}
					else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						MCComponet.MeshCollider->InitializeMeshCollider();

						if (!RBComponent.DynamicRigidBody->GetRigidBodyCollider())
						{
							RBComponent.DynamicRigidBody->SetRigidBodyCollider(MCComponet.MeshCollider);
						}

						RBComponent.DynamicRigidBody->InitializeDynamicRigidBody();

						this->PhysicsWorld->addActor(*RBComponent.DynamicRigidBody->GetRigidBodyObject());
					}
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianNativeScriptComponent>();
			view.each([this](const auto& e, GuardianNativeScriptComponent& SComponent)
			{
				SComponent.EntityInstance = SComponent.InstantiateScript();
				SComponent.EntityInstance->EntityObject = GuardianEntity(e,
					this->SceneRegistry.get<GuardianTagComponent>(e).Tag, this);
			});
		}

		{
			GuardianScriptEngine::StartRuntime(this);

			auto view = this->SceneRegistry.view<GuardianScriptComponent>();
			view.each([this](const entt::entity& e, GuardianScriptComponent& SComponent)
			{
				auto& entity = this->SceneEntityList[e];

				GuardianScriptEngine::OnInitializeEntity(entity);
			});
		}
	}

	void GuardianScene::UpdateRuntimeScene(GuardianTimestep deltaTime)
	{
		GuardianRenderer::SetRenderingRenderGraphCamera(this->SceneName + " Scene Rendering", *this->RuntimeCamera);
		GuardianRenderer::SetRenderingRenderGraphClearColor(this->SceneName + " Scene Rendering", GVector3(0.1f, 0.1f, 0.1f));

		{
			this->PhysicsWorld->simulate(deltaTime.GetSecond());
			
			this->PhysicsWorld->fetchResults(true);
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianPointLightComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianPointLightComponent& PLComponent)
			{
				PLComponent.LightProperties.LightPosition = TComponent.Position;

				GuardianLightSystem::SubmitPointLight(PLComponent.LightProperties);
			});
		}
		GuardianLightProperties LightProperties;
		int index = 0;
		while (!GuardianLightSystem::IsPointLightListEmpty())
		{
			GuardianPointLightProperties pointLight = GuardianLightSystem::ReadPointLight();
			LightProperties.PointLightList[index] = pointLight;
			index++;
		}
		LightProperties.LightNumber = index;
		LightProperties.CameraPosition = this->RuntimeCamera->Position;

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianRigidBodyComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianRigidBodyComponent& RBComponent)
				{
					if (RBComponent.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						TComponent.Position = RBComponent.StaticRigidBody->GetRigidBodyTransform().Position;
						GVector4 Quaternion = RBComponent.StaticRigidBody->GetRigidBodyTransform().Quaternion;
						TComponent.Rotation = GVector4::QuaternionToEuler({ Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w });
					}
					else if (RBComponent.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						TComponent.Position = RBComponent.DynamicRigidBody->GetRigidBodyTransform().Position;
						GVector4 Quaternion = RBComponent.DynamicRigidBody->GetRigidBodyTransform().Quaternion;
						TComponent.Rotation = GVector4::QuaternionToEuler({ Quaternion.x, Quaternion.y, Quaternion.z, Quaternion.w });
					}
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianNativeScriptComponent>();
			view.each([this](const auto& e, GuardianNativeScriptComponent& SComponent)
				{
					SComponent.EntityInstance->Update(16.667f);
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianScriptComponent>();
			view.each([this](const entt::entity& e, GuardianScriptComponent& SComponent)
				{
					auto& entity = this->SceneEntityList[e];

					GuardianScriptEngine::OnUpdateEntity(entity);
				});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianCameraComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianCameraComponent& CComponent)
			{
				CComponent.Position = TComponent.Position;
				CComponent.Direction = TComponent.Rotation;

				this->RuntimeCamera->Position = CComponent.Position;
				this->RuntimeCamera->Direction = CComponent.Direction;
				this->RuntimeCamera->IsFreelook = CComponent.IsFreelook;
				this->RuntimeCamera->Projection.FarZ = CComponent.Projection.FarZ;
				this->RuntimeCamera->Projection.NearZ = CComponent.Projection.NearZ;
				this->RuntimeCamera->Projection.FOV = CComponent.Projection.FOV;
			});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianMeshComponent>();
			view.each([this, &LightProperties](const auto& e,
				GuardianTransformComponent& TComponent, GuardianMeshComponent& MComponent)
			{
				MComponent.Mesh->UpdateMeshTransform(TComponent.GetTransformMatrix());
				MComponent.Mesh->UpdateMeshLighting(LightProperties);

				MComponent.Mesh->SubmitToRenderer(this->SceneName + " Scene Rendering");
			});
		}
	}

	void GuardianScene::StopRuntime()
	{
		this->PhysicsWorld->release();
		this->PhysicsWorld = null;

		{
			auto view = this->SceneRegistry.view<GuardianNativeScriptComponent>();
			view.each([this](const auto& e, GuardianNativeScriptComponent& SComponent)
			{
				SComponent.EntityInstance->Release();
				SComponent.ReleaseScript(&SComponent);
			});
		}

		{
			GuardianScriptEngine::StopRuntime();
		}

		this->RemoveAllEntity();
		this->Deserialize("Temp.gdata", false);
		std::filesystem::remove("Temp.gdata");

		this->SceneState = GE_SCENE_EDIT;
	}

	std::shared_ptr<GuardianEntity> GuardianScene::CreateEntity(const GString& name)
	{
		auto Entity = std::make_shared<GuardianEntity>(this->SceneRegistry.create(), name, this);

		if (this->SceneEntityList[Entity->GetEntityHandle()].get())
		{
			this->RemoveEntity(Entity->GetEntityHandle());
		}
		this->SceneEntityList[Entity->GetEntityHandle()] = Entity;

		this->SceneEntityList[Entity->GetEntityHandle()]->AddComponent<GuardianTagComponent>(name);

		return this->SceneEntityList[Entity->GetEntityHandle()];
	}

	std::shared_ptr<GuardianEntity> GuardianScene::CreateEntity(const GString& name, const GuardianUUID& uuid)
	{
		auto Entity = std::make_shared<GuardianEntity>(this->SceneRegistry.create(), name, uuid, this);

		if (this->SceneEntityList[Entity->GetEntityHandle()].get())
		{
			this->RemoveEntity(Entity->GetEntityHandle());
		}
		this->SceneEntityList[Entity->GetEntityHandle()] = Entity;

		this->SceneEntityList[Entity->GetEntityHandle()]->AddComponent<GuardianTagComponent>(name);

		return this->SceneEntityList[Entity->GetEntityHandle()];
	}

	void GuardianScene::RemoveEntity(const entt::entity& handle)
	{
		this->SceneRegistry.destroy(this->SceneEntityList[handle]->GetEntityHandle());
		this->SceneEntityList.erase(handle);
	}

	void GuardianScene::RemoveAllEntity()
	{
		for (auto& entity : this->SceneEntityList)
		{
			this->SceneRegistry.destroy(entity.second->GetEntityHandle());
			entity.second.reset();
			entity.second = null;
		}
		this->SceneEntityList.clear();
	}

	const GuardianSceneState& GuardianScene::GetSceneState() const
	{
		return this->SceneState;
	}

	std::shared_ptr<GuardianEntity> GuardianScene::GetEntity(const GuardianUUID& uuid)
	{
		for (auto& entity : this->SceneEntityList)
		{
			if (entity.second->GetEntityId() == uuid)
			{
				return entity.second;
				break;
			}
		}

		return null;
	}

	void GuardianScene::CreatePhysicsWorld()
	{
		PxSceneDesc SceneDesc(GuardianPhysicsEngine::PhysicsObject->getTolerancesScale());
		SceneDesc.gravity = PxVec3(this->SceneGravity.x, this->SceneGravity.y, this->SceneGravity.z);
		SceneDesc.cpuDispatcher = GuardianPhysicsEngine::PhysicsCpuDispatcher;
		SceneDesc.filterShader = PxDefaultSimulationFilterShader;
		SceneDesc.cudaContextManager = GuardianPhysicsEngine::GetPhysicsCudaManager();
		SceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
		SceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		SceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
		this->PhysicsWorld = GuardianPhysicsEngine::PhysicsObject->createScene(SceneDesc);
		if (!this->PhysicsWorld)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics world!");
		}

		PxPvdSceneClient* DebuggerClient = this->PhysicsWorld->getScenePvdClient();
		if (DebuggerClient)
		{
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			DebuggerClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void GuardianScene::ReadSerializationData(const GString& serializationData)
	{
		YAML::Node SceneData = YAML::Load(serializationData);

		auto entities = SceneData["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				auto TagComponent = entity["Tag Component"];
				GString name = TagComponent["Tag"].as<GString>();
				auto LoadedEntity = this->CreateEntity(name, GuardianUUID(entity["Entity"].as<uint64_t>()));

				auto TransformComponent = entity["Transform Component"];
				if (TransformComponent)
				{
					auto& Transform = LoadedEntity->AddComponent<GuardianTransformComponent>();
					Transform.Position = TransformComponent["Position"].as<GVector3>();
					Transform.Rotation = TransformComponent["Rotation"].as<GVector3>();
					Transform.Scale = TransformComponent["Scale"].as<GVector3>();
				}

				auto CameraComponent = entity["Camera Component"];
				if (CameraComponent)
				{
					auto& Camera = LoadedEntity->AddComponent<GuardianCameraComponent>();
					Camera.Position = CameraComponent["Position"].as<GVector3>();
					Camera.Direction = CameraComponent["Direction"].as<GVector3>();
					Camera.IsFreelook = CameraComponent["Is Freelook"].as<bool>();
					auto Projection = CameraComponent["Perspective Projection"];
					Camera.Projection.FOV = Projection["FOV"].as<float>();
					Camera.Projection.Aspect = Projection["Aspect"].as<float>();
					Camera.Projection.NearZ = Projection["NearZ"].as<float>();
					Camera.Projection.FarZ = Projection["FarZ"].as<float>();
				}

				auto ScriptComponent = entity["Script Component"];
				if (ScriptComponent)
				{
					auto& Script = LoadedEntity->AddComponent<GuardianScriptComponent>();
					Script.ClassName = ScriptComponent["Class"].as<GString>();
				}

				auto PointLightComponent = entity["Point Light Component"];
				if (PointLightComponent)
				{
					auto& PointLight = LoadedEntity->AddComponent<GuardianPointLightComponent>().LightProperties;
					PointLight.LightPosition = PointLightComponent["Light Position"].as<GVector3>();
					PointLight.LightStrength = PointLightComponent["Light Strength"].as<float>();
					PointLight.LightColor = PointLightComponent["Light Color"].as<GVector3>();
				}

				auto MeshComponent = entity["Mesh Component"];
				if (MeshComponent)
				{
					auto& MeshC = LoadedEntity->AddComponent<GuardianMeshComponent>();
					GString MeshName = MeshComponent["Mesh Name"].as<GString>();
					MeshC.Mesh = std::make_shared<GuardianMesh>();
					MeshC.Mesh->InitializeMesh(MeshName, GuardianAssetSystem::GetMeshAsset(MeshName).GetMeshAssetData());
				}

				auto SphereColliderComponent = entity["Sphere Collider Component"];
				if (SphereColliderComponent)
				{
					auto& SphereCollider = LoadedEntity->AddComponent<GuardianSphereColliderComponent>().SphereCollider;
					auto Radius = SphereColliderComponent["Radius"].as<float>();
					auto PhysicsMaterial = SphereColliderComponent["Physics Material"];
					float StaticFriction = PhysicsMaterial["Static Friction"].as<float>();
					float DynamicFriction = PhysicsMaterial["Dynamic Friction"].as<float>();
					float Restitution = PhysicsMaterial["Restitution"].as<float>();
					SphereCollider->SetColliderProperties({ Radius });
					SphereCollider->SetColliderMaterial(GuardianPhysicsMaterial(StaticFriction, DynamicFriction, Restitution));
				}

				auto BoxColliderComponent = entity["Box Collider Component"];
				if (BoxColliderComponent)
				{
					auto& BoxCollider = LoadedEntity->AddComponent<GuardianBoxColliderComponent>().BoxCollider;
					auto HalfSize = BoxColliderComponent["Half Size"].as<GVector3>();
					auto PhysicsMaterial = BoxColliderComponent["Physics Material"];
					float StaticFriction = PhysicsMaterial["Static Friction"].as<float>();
					float DynamicFriction = PhysicsMaterial["Dynamic Friction"].as<float>();
					float Restitution = PhysicsMaterial["Restitution"].as<float>();
					BoxCollider->SetColliderProperties({ HalfSize });
					BoxCollider->SetColliderMaterial(GuardianPhysicsMaterial(StaticFriction, DynamicFriction, Restitution));
				}

				auto CapsuleColliderComponent = entity["Capsule Collider Component"];
				if (CapsuleColliderComponent)
				{
					auto& CapsuleCollider = LoadedEntity->AddComponent<GuardianCapsuleColliderComponent>().CapsuleCollider;
					auto Radius = CapsuleColliderComponent["Radius"].as<float>();
					auto HalfHeight = CapsuleColliderComponent["Half Height"].as<float>();
					auto PhysicsMaterial = CapsuleColliderComponent["Physics Material"];
					float StaticFriction = PhysicsMaterial["Static Friction"].as<float>();
					float DynamicFriction = PhysicsMaterial["Dynamic Friction"].as<float>();
					float Restitution = PhysicsMaterial["Restitution"].as<float>();
					CapsuleCollider->SetColliderProperties({ Radius, HalfHeight });
					CapsuleCollider->SetColliderMaterial(GuardianPhysicsMaterial(StaticFriction, DynamicFriction, Restitution));
				}

				auto RigidBodyComponent = entity["RigidBody Component"];
				if (RigidBodyComponent)
				{
					auto& component = LoadedEntity->AddComponent<GuardianRigidBodyComponent>();
					component.RigidBodyType = (GuardianRigidBodyType)RigidBodyComponent["Type"].as<unsigned int>();
					if (component.RigidBodyType == GE_RIGIDBODY_DYNAMIC)
					{
						auto& DynamicRigidBody = LoadedEntity->GetComponent<GuardianRigidBodyComponent>().DynamicRigidBody;
						float density = RigidBodyComponent["Density"].as<float>();
						auto& transform = LoadedEntity->GetComponent<GuardianTransformComponent>();
						DynamicRigidBody->SetRigidBodyTransform(transform);
						DynamicRigidBody->SetRigidBodyDensity(density);
					}
					else if (component.RigidBodyType == GE_RIGIDBODY_STATIC)
					{
						auto& StaticRigidBody = LoadedEntity->GetComponent<GuardianRigidBodyComponent>().StaticRigidBody;
						auto& transform = LoadedEntity->GetComponent<GuardianTransformComponent>();
						StaticRigidBody->SetRigidBodyTransform(transform);
					}
				}
			}
		}
	}

	const GString GuardianScene::WriteSerializationData()
	{
		YAML::Emitter SceneOutput;
		SceneOutput << YAML::BeginMap;
		SceneOutput << YAML::Key << "Scene";
		SceneOutput << YAML::Value << this->SceneName;
		SceneOutput << YAML::Key << "Entities";
		SceneOutput << YAML::Value << YAML::BeginSeq;

		for (auto& entity : this->SceneEntityList)
		{
			this->SaveEntity(SceneOutput, entity.second);
		}
		
		SceneOutput << YAML::EndSeq;
		SceneOutput << YAML::EndMap;

		GString OutputData = SceneOutput.c_str();
		return OutputData;
	}

	void GuardianScene::SaveEntity(YAML::Emitter& output, std::shared_ptr<GuardianEntity> entity)
	{
		output << YAML::BeginMap;
		output << YAML::Key << "Entity";
		output << YAML::Value << entity->GetEntityId();

		if (entity->HasComponent<GuardianTagComponent>())
		{
			output << YAML::Key << "Tag Component";
			output << YAML::BeginMap;

			auto& tag = entity->GetComponent<GuardianTagComponent>().Tag;
			output << YAML::Key << "Tag" << YAML::Value << tag;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianTransformComponent>())
		{
			output << YAML::Key << "Transform Component";
			output << YAML::BeginMap;

			auto& Position = entity->GetComponent<GuardianTransformComponent>().Position;
			output << YAML::Key << "Position" << YAML::Value << Position;

			auto& Rotation = entity->GetComponent<GuardianTransformComponent>().Rotation;
			output << YAML::Key << "Rotation" << YAML::Value << Rotation;

			auto& Scale = entity->GetComponent<GuardianTransformComponent>().Scale;
			output << YAML::Key << "Scale" << YAML::Value << Scale;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianCameraComponent>())
		{
			output << YAML::Key << "Camera Component";
			output << YAML::BeginMap;

			auto& Position = entity->GetComponent<GuardianCameraComponent>().Position;
			output << YAML::Key << "Position" << YAML::Value << Position;

			auto& Direction = entity->GetComponent<GuardianCameraComponent>().Direction;
			output << YAML::Key << "Direction" << YAML::Value << Direction;

			auto& IsFreelook = entity->GetComponent<GuardianCameraComponent>().IsFreelook;
			output << YAML::Key << "Is Freelook" << YAML::Value << IsFreelook;

			auto& Projection = entity->GetComponent<GuardianCameraComponent>().Projection;
			output << YAML::Key << "Perspective Projection";
			output << YAML::BeginMap;
			output << YAML::Key << "FOV" << YAML::Value << Projection.FOV;
			output << YAML::Key << "Aspect" << YAML::Value << Projection.Aspect;
			output << YAML::Key << "NearZ" << YAML::Value << Projection.NearZ;
			output << YAML::Key << "FarZ" << YAML::Value << Projection.FarZ;
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianScriptComponent>())
		{
			output << YAML::Key << "Script Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Class" << YAML::Value <<
				entity->GetComponent<GuardianScriptComponent>().ClassName;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianPointLightComponent>())
		{
			output << YAML::Key << "Point Light Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Light Position" <<
				entity->GetComponent<GuardianPointLightComponent>().LightProperties.LightPosition;
			output << YAML::Key << "Light Strength" <<
				entity->GetComponent<GuardianPointLightComponent>().LightProperties.LightStrength;
			output << YAML::Key << "Light Color" <<
				entity->GetComponent<GuardianPointLightComponent>().LightProperties.LightColor;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianMeshComponent>())
		{
			output << YAML::Key << "Mesh Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Mesh Name" << YAML::Value <<
				entity->GetComponent<GuardianMeshComponent>().Mesh->GetMeshName();

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianSphereColliderComponent>())
		{
			output << YAML::Key << "Sphere Collider Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Radius" << YAML::Value <<
				entity->GetComponent<GuardianSphereColliderComponent>().SphereCollider->GetColliderProperties().Radius;

			auto& material = entity->GetComponent<GuardianSphereColliderComponent>().SphereCollider->GetColliderMaterial();
			output << YAML::Key << "Physics Material";
			output << YAML::BeginMap;
			output << YAML::Key << "Static Friction" << YAML::Value << material.GetStaticFriction();
			output << YAML::Key << "Dynamic Friction" << YAML::Value << material.GetDynamicFriction();
			output << YAML::Key << "Restitution" << YAML::Value << material.GetRestitution();
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianBoxColliderComponent>())
		{
			output << YAML::Key << "Box Collider Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Half Size" << YAML::Value <<
				entity->GetComponent<GuardianBoxColliderComponent>().BoxCollider->GetColliderProperties().BoxHalfsize;

			auto& material = entity->GetComponent<GuardianBoxColliderComponent>().BoxCollider->GetColliderMaterial();
			output << YAML::Key << "Physics Material";
			output << YAML::BeginMap;
			output << YAML::Key << "Static Friction" << YAML::Value << material.GetStaticFriction();
			output << YAML::Key << "Dynamic Friction" << YAML::Value << material.GetDynamicFriction();
			output << YAML::Key << "Restitution" << YAML::Value << material.GetRestitution();
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianCapsuleColliderComponent>())
		{
			output << YAML::Key << "Capsule Collider Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Radius" << YAML::Value <<
				entity->GetComponent<GuardianCapsuleColliderComponent>().CapsuleCollider->GetColliderProperties().Radius;
			output << YAML::Key << "Half Height" << YAML::Value <<
				entity->GetComponent<GuardianCapsuleColliderComponent>().CapsuleCollider->GetColliderProperties().HalfHeight;

			auto& material = entity->GetComponent<GuardianCapsuleColliderComponent>().CapsuleCollider->GetColliderMaterial();
			output << YAML::Key << "Physics Material";
			output << YAML::BeginMap;
			output << YAML::Key << "Static Friction" << YAML::Value << material.GetStaticFriction();
			output << YAML::Key << "Dynamic Friction" << YAML::Value << material.GetDynamicFriction();
			output << YAML::Key << "Restitution" << YAML::Value << material.GetRestitution();
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		if (entity->HasComponent<GuardianRigidBodyComponent>())
		{
			output << YAML::Key << "RigidBody Component";
			output << YAML::BeginMap;

			output << YAML::Key << "Type" << YAML::Value << (unsigned int)
				entity->GetComponent<GuardianRigidBodyComponent>().RigidBodyType;

			if (entity->GetComponent<GuardianRigidBodyComponent>().RigidBodyType == GE_RIGIDBODY_DYNAMIC)
			{
				output << YAML::Key << "Density" << YAML::Value <<
					entity->GetComponent<GuardianRigidBodyComponent>().DynamicRigidBody->GetRigidBodyDensity();
			}

			output << YAML::EndMap;
		}

		output << YAML::EndMap;
	}

	void GuardianScene::UpdateProjectionAspect(float newWidth, float newHeight)
	{
		if (this->SceneState == GE_SCENE_EDIT)
		{
			this->EditorCamera->Projection.Aspect = newWidth / newHeight;
		}
		else if (this->SceneState == GE_SCENE_RUNTIME)
		{
			this->RuntimeCamera->Projection.Aspect = newWidth / newHeight;
		}
	}
}
