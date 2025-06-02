#include "GuardianSceneGraph.h"
#include "../../ECS/Scene/GuardianScene.h"
#include "../../ECS/Entity/GuardianEntity.h"

namespace GE
{
	GuardianSceneGraph::GuardianSceneGraph()
	{
		this->GraphScene = null;
		this->GraphClearColor = { 0.0f, 0.0f, 0.0f };

		this->GraphShaderGroup = null;
		
		this->GraphFramebuffer = null;
		this->GraphCamera = {};
		this->RenderableQueueList.clear();
		this->DepthGraphList.clear();
	}

	GuardianSceneGraph::GuardianSceneGraph(GuardianScene* scene, int width, int height)
	{
		this->InitializeSceneGraph(scene, width, height);
	}

	GuardianSceneGraph::GuardianSceneGraph(const GuardianSceneGraph& other)
	{
		this->GraphScene = other.GraphScene;
		this->GraphClearColor = other.GraphClearColor;

		this->GraphShaderGroup = other.GraphShaderGroup;

		this->GraphFramebuffer = other.GraphFramebuffer;
		this->GraphCamera = other.GraphCamera;
		this->RenderableQueueList = other.RenderableQueueList;
		this->DepthGraphList = other.DepthGraphList;
	}

	GuardianSceneGraph::~GuardianSceneGraph()
	{
		this->GraphScene = null;
		this->GraphClearColor = { 0.0f, 0.0f, 0.0f };

		this->GraphShaderGroup = null;

		this->GraphFramebuffer.reset();
		this->GraphCamera = {};
		this->RenderableQueueList.clear();
		this->DepthGraphList.clear();
	}

	void GuardianSceneGraph::InitializeSceneGraph(GuardianScene* scene, int width, int height)
	{
		this->GraphScene = scene;

		this->GraphFramebuffer = std::make_shared<GuardianFramebuffer>(
			GuardianGraphicsRegistry::GetCurrentGraphics(), width, height);
		this->GraphShaderGroup = GuardianShaderSystem::GetShaderGroup(GuardianShaderSystem::PBR_MAIN_SHADER);
	}

	void GuardianSceneGraph::SetGraphClearColor(const GVector3& color)
	{
		this->GraphClearColor = color;
	}

	void GuardianSceneGraph::SetGraphShaderGroup(const GString& groupName)
	{
		this->GraphShaderGroup = GuardianShaderSystem::GetShaderGroup(GuardianShaderSystem::PBR_MAIN_SHADER);
	}

	void GuardianSceneGraph::AddDepthGraph(const GString& name, int width, int height)
	{
		if (this->DepthGraphList.count(name) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("This scene graph already has the depth graph named : '{}'!", name));
		}

		this->DepthGraphList[name] = std::make_shared<GuardianDepthGraph>(name, width, height);
	}

	void GuardianSceneGraph::SetDepthGraphCamera(const GString& name, const GuardianCamera& camera)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("This render graph hasn't the depth graph named : '{}'!", name));
		}

		this->DepthGraphList[name]->SetGraphCamera(camera);
	}

	const bool GuardianSceneGraph::HasDepthGraph(const GString& name) const noexcept
	{
		return this->DepthGraphList.count(name) > 0;
	}

	std::shared_ptr<GuardianDepthGraph> GuardianSceneGraph::GetDepthGraph(const GString& name)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No depth graph found in this render graph named : '{}'!", name));
		}

		return this->DepthGraphList[name];
	}

	std::shared_ptr<GuardianDepthStencil> GuardianSceneGraph::GetDepthGraphDepthStencil(const GString& name)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No depth graph found in this render graph named : '{}'!", name));
		}

		return this->DepthGraphList[name]->GetGraphDepthStencil();
	}

	void GuardianSceneGraph::SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable)
	{
		this->RenderableQueueList[level].push(renderable);

		if (level != GE_SUBMIT_SPECIALLY && level != GE_SUBMIT_DEFAULT2D &&
			level != GE_SUBMIT_TRANSPARENT2D && level != GE_SUBMIT_GUI2D)
		{
			for (auto& depthGraph : this->DepthGraphList)
			{
				depthGraph.second->SubmitRenderable(renderable);
			}
		}
	}

	void GuardianSceneGraph::Render()
	{
		switch (this->GraphScene->SceneState)
		{
			case GE_SCENE_EDIT:
			{
				this->RenderEditScene();
				break;
			}

			case GE_SCENE_RUNTIME:
			{
				this->RenderRuntimeScene();
				break;
			}
		}

		if (!this->GraphShaderGroup)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This render graph hasn't been set shaders!");
		}

		this->GraphFramebuffer->ApplyFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), this->GraphClearColor);

		GuardianDepthGraph::DepthMapSampler->Apply();

		for (UINT i = 0; i <= (UINT)GE_SUBMIT_SPECIALLY; i++)
		{
			while (!this->RenderableQueueList[(GuardianSubmitPassLevel)i].empty())
			{
				auto& renderable = this->RenderableQueueList[(GuardianSubmitPassLevel)i].front();

				renderable->GetTransformConstantBuffer()->UpdateData(
					{ renderable->GetTransformConstantBuffer()->GetConstantBufferData().WorldTransformMatrix,
					this->GraphCamera.GetViewMatrix(), this->GraphCamera.GetProjectionMatrix() });
				renderable->Update();

				if (!renderable->UseSpecialShader)
				{
					for (auto& depthGraph : this->DepthGraphList)
					{
						GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDeviceContext()->
							PSSetShaderResources(5, 1, depthGraph.second->GetGraphDepthStencil()->GetDepthStencilShaderResource().GetAddressOf());
					}

					this->GraphShaderGroup->Apply();
				}

				renderable->Render();

				this->RenderableQueueList[(GuardianSubmitPassLevel)i].pop();
			}
		}

		for (auto& depthGraph : this->DepthGraphList)
		{
			depthGraph.second->Render();
		}
	}

	void GuardianSceneGraph::ResizeSceneGraph(int width, int height)
	{
		this->GraphFramebuffer->ResizeFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), width, height);
	}

	std::shared_ptr<GuardianFramebuffer> GuardianSceneGraph::GetSceneGraphFramebuffer()
	{
		return this->GraphFramebuffer;
	}

	void GuardianSceneGraph::RenderEditScene()
	{
		this->GraphCamera = *this->GraphScene->EditorCamera;

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianPointLightComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianPointLightComponent& PLComponent)
			{
				PLComponent.LightProperties.LightPosition = TComponent.Position;
				PLComponent.LightProperties.LightViewMatrix =
					XMMatrixTranspose(GuardianCamera(TComponent.Position, TComponent.Rotation, GuardianPerspectiveProjection(60.0f, 1024.0f / 1024.0f, 3.0f, 1000.0f)).GetViewMatrix() *
					GuardianCamera(TComponent.Position, TComponent.Rotation, GuardianPerspectiveProjection(60.0f, 1024.0f / 1024.0f, 3.0f, 1000.0f)).GetProjectionMatrix());

				if (!PLComponent.LightMesh)
				{
					PLComponent.LightMesh = std::make_shared<GuardianMesh>();
					PLComponent.LightMesh->InitializeMesh("Light", "../Guardian Engine/Assets/Models/Light/Light.obj");
				}
				PLComponent.LightMesh->SubmitToRenderer(this->GraphScene->SceneName, GE_SUBMIT_SPECIALLY);

				GuardianLightSystem::SubmitPointLight(PLComponent.LightProperties);
				GuardianTransform transform;
				transform.Position = TComponent.Position;
				transform.Rotation = TComponent.Rotation;
				transform.Quaternion = TComponent.Quaternion;
				transform.Scale = GVector3(1.0f, 1.0f, 1.0f);
				PLComponent.LightMesh->UpdateMeshTransform(transform.GetTransformMatrix());

				if (!this->HasDepthGraph(std::to_string((unsigned long long)PLComponent.LightId)))
				{
					this->AddDepthGraph(std::to_string((unsigned long long)PLComponent.LightId), 1024, 1024);
				}
				this->GetDepthGraph(std::to_string((unsigned long long)PLComponent.LightId))->SetGraphCamera(
					GuardianCamera(transform.Position, transform.Rotation, GuardianPerspectiveProjection(60.0f, 1024.0f / 1024.0f, 3.0f, 1000.0f)));
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
		LightProperties.CameraPosition = this->GraphCamera.Position;

		if (!this->GraphScene->SceneGrid)
		{
			this->GraphScene->SceneGrid = std::make_shared<GuardianMesh>();
			this->GraphScene->SceneGrid->InitializeMesh("Grid", "../Guardian Engine/Assets/Models/Grid/Grid.obj");
		}
		this->GraphScene->SceneGrid->SubmitToRenderer(this->GraphScene->SceneName, GE_SUBMIT_SPECIALLY);
		this->GraphScene->SceneGrid->UpdateMeshTransform(GuardianTransform().GetTransformMatrix());
		this->GraphScene->SceneGrid->UpdateMeshLighting(LightProperties);

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianSphereColliderComponent>();
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

					if (this->GraphScene->SceneEntityList[e]->IsSelected())
					{
						GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->GraphScene->SceneName, SCComponent.SphereGeometry);
					}
				});
		}

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianBoxColliderComponent>();
			view.each([this](const entt::entity& e, GuardianTransformComponent& TComponent,
				GuardianBoxColliderComponent& BCComponent)
				{
					if (!BCComponent.BoxGeometry->IsInitialized())
					{
						BCComponent.BoxGeometry->InitializeGeometry(GE_GEOMETRY_BOX);
					}

					GuardianTransform transform = GuardianTransform(TComponent.Position, TComponent.Rotation,
						TComponent.Quaternion, BCComponent.BoxCollider->GetColliderProperties().BoxHalfsize * 2.0f);
					BCComponent.BoxGeometry->UpdateGeometryTransform(transform.GetTransformMatrix());

					if (this->GraphScene->SceneEntityList[e]->IsSelected())
					{
						GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->GraphScene->SceneName, BCComponent.BoxGeometry);
					}
				});
		}

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianCapsuleColliderComponent>();
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

					if (this->GraphScene->SceneEntityList[e]->IsSelected())
					{
						GuardianRenderer::SubmitRenderable(GE_SUBMIT_SPECIALLY, this->GraphScene->SceneName, CCComponent.CapsuleGeometry);
					}
				});
		}

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianCameraComponent>();
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
				CComponent.CameraMesh->SubmitToRenderer(this->GraphScene->SceneName, GE_SUBMIT_SPECIALLY);

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
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianMeshComponent>();
			view.each([this, &LightProperties](const auto& e,
				GuardianTransformComponent& TComponent, GuardianMeshComponent& MComponent)
				{
					MComponent.Mesh->UpdateMeshTransform(TComponent.GetTransformMatrix());
					MComponent.Mesh->UpdateMeshLighting(LightProperties);

					MComponent.Mesh->SubmitToRenderer(this->GraphScene->SceneName);
				});
		}
	}

	void GuardianSceneGraph::RenderRuntimeScene()
	{
		this->GraphCamera = *this->GraphScene->RuntimeCamera;

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianPointLightComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianPointLightComponent& PLComponent)
			{
				PLComponent.LightProperties.LightPosition = TComponent.Position;

				GuardianLightSystem::SubmitPointLight(PLComponent.LightProperties);

				if (!this->HasDepthGraph(std::to_string((unsigned long long)PLComponent.LightId)))
				{
					this->AddDepthGraph(std::to_string((unsigned long long)PLComponent.LightId), 1024, 1024);
				}
				this->GetDepthGraph(std::to_string((unsigned long long)PLComponent.LightId))->SetGraphCamera(
					GuardianCamera(TComponent.Position, TComponent.Rotation, GuardianPerspectiveProjection(60.0f, 1024.0f / 1024.0f, 5.0f, 1000.0f)));
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
		LightProperties.CameraPosition = this->GraphCamera.Position;

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianCameraComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianCameraComponent& CComponent)
				{
					CComponent.Position = TComponent.Position;
					CComponent.Direction = TComponent.Rotation;

					this->GraphScene->RuntimeCamera->Position = CComponent.Position;
					this->GraphScene->RuntimeCamera->Direction = CComponent.Direction;
					this->GraphScene->RuntimeCamera->IsFreelook = CComponent.IsFreelook;
					this->GraphScene->RuntimeCamera->Projection.FarZ = CComponent.Projection.FarZ;
					this->GraphScene->RuntimeCamera->Projection.NearZ = CComponent.Projection.NearZ;
					this->GraphScene->RuntimeCamera->Projection.FOV = CComponent.Projection.FOV;
				});
		}

		{
			auto view = this->GraphScene->SceneRegistry.view<GuardianTransformComponent, GuardianMeshComponent>();
			view.each([this, &LightProperties](const auto& e,
				GuardianTransformComponent& TComponent, GuardianMeshComponent& MComponent)
			{
				MComponent.Mesh->UpdateMeshTransform(TComponent.GetTransformMatrix());
				MComponent.Mesh->UpdateMeshLighting(LightProperties);

				MComponent.Mesh->SubmitToRenderer(this->GraphScene->SceneName);
			});
		}
	}
}
