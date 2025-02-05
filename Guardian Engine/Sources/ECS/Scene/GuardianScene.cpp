#include "GuardianScene.h"
#include "../Entity/GuardianEntity.h"
#include "../Entity/GuardianScriptableEntity.h"
#include "../../Application/GuardianApplication.h"

namespace YAML
{
	template<>
	struct convert<guardian::GVector3>
	{
		static Node encode(const guardian::GVector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, guardian::GVector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<guardian::GVector4>
	{
		static Node encode(const guardian::GVector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, guardian::GVector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};
}

namespace guardian
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const GVector3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const GVector4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}

	GuardianScene::GuardianScene()
	{
		this->SceneEntityList.clear();
		this->CurrentScenePath = "";
		this->EditorCamera = std::make_shared<GuardianCamera>(GVector3(0.0f, 1.0f, -2.0f), 
			GVector3(0.0f, 0.0f, 0.0f), GuardianPerspectiveProjection(60.0f, 16.0f / 9.0f, 0.001f, 10000.0f));
		this->RuntimeCamera = std::make_shared<GuardianCamera>();
		this->ShouldOperateCamera = false;
		this->SceneGrid = null;
		this->SceneState = GE_SCENE_EDIT;
	}

	GuardianScene::GuardianScene(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->EditorCamera = std::make_shared<GuardianCamera>(GVector3(0.0f, 1.0f, -2.0f),
			GVector3(0.0f, 0.0f, 0.0f), GuardianPerspectiveProjection(60.0f, 16.0f / 9.0f, 0.001f, 10000.0f));
		this->ShouldOperateCamera = false;
		this->SceneGrid = null;
		this->RuntimeCamera = std::make_shared<GuardianCamera>();
		this->SceneState = GE_SCENE_EDIT;

		this->InitializeScene(graphics);
	}

	GuardianScene::GuardianScene(const GuardianScene& other)
	{
		this->SceneState = other.SceneState;
		this->SceneEntityList = other.SceneEntityList;
		this->CurrentScenePath = other.CurrentScenePath;
		this->EditorCamera = other.EditorCamera;
		this->RuntimeCamera = other.RuntimeCamera;
		this->ShouldOperateCamera = other.ShouldOperateCamera;
	}

	void GuardianScene::InitializeScene(std::shared_ptr<GuardianGraphics> graphics)
	{
		if (this->SceneState == GE_SCENE_RUNTIME)
		{
			this->StopRuntime();
		}

		this->RemoveAllEntity();

		GString sceneFilePath = GuardianFileDialog::OpenFile("Guardian Engine Scene (*.gscene)\0*.gscene\0");

		std::ifstream SceneFile(sceneFilePath);
		std::stringstream SceneFileStringStream;
		SceneFileStringStream << SceneFile.rdbuf();

		YAML::Node SceneData = YAML::Load(SceneFileStringStream.str());
		if (!SceneData["Scene"])
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to load scene : '" + sceneFilePath + "' !");
		}
		this->CurrentScenePath = sceneFilePath;

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

				auto ModelComponent = entity["Model Component"];
				if (ModelComponent)
				{
					auto& Model = LoadedEntity->AddComponent<GuardianModelComponent>();
					auto ModelFilePath = ModelComponent["File Path"].as<GString>();
					Model.InitializeModel(graphics, ModelFilePath);
				}
			}
		}
	}

	void GuardianScene::InitializeSceneAs(std::shared_ptr<GuardianGraphics> graphics, const GString& sceneFilePath)
	{
		if (this->SceneState == GE_SCENE_RUNTIME)
		{
			this->StopRuntime();
		}

		this->RemoveAllEntity();

		std::ifstream SceneFile(sceneFilePath);
		std::stringstream SceneFileStringStream;
		SceneFileStringStream << SceneFile.rdbuf();

		if (std::filesystem::path(sceneFilePath).extension() != ".gscene")
		{
			return;
		}

		YAML::Node SceneData = YAML::Load(SceneFileStringStream.str());
		if (!SceneData["Scene"])
		{
			throw GUARDIAN_ERROR_EXCEPTION(("Failed to load scene : '" + sceneFilePath + "' !").c_str());
		}
		this->CurrentScenePath = sceneFilePath;

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

				auto ModelComponent = entity["Model Component"];
				if (ModelComponent)
				{
					auto& Model = LoadedEntity->AddComponent<GuardianModelComponent>();
					auto ModelFilePath = ModelComponent["File Path"].as<GString>();
					Model.InitializeModel(graphics, ModelFilePath);
				}
			}
		}
	}

	void GuardianScene::SaveScene()
	{
		YAML::Emitter SceneOutput;
		SceneOutput << YAML::BeginMap;
		SceneOutput << YAML::Key << "Scene";
		SceneOutput << YAML::Value << "Unnamed";
		SceneOutput << YAML::Key << "Entities";
		SceneOutput << YAML::Value << YAML::BeginSeq;
		for (auto& entity : this->SceneEntityList)
		{
			this->SaveEntity(SceneOutput, entity.second);
		}
		SceneOutput << YAML::EndSeq;
		SceneOutput << YAML::EndMap;

		if (this->CurrentScenePath == "")
		{
			this->CurrentScenePath = GuardianFileDialog::SaveFile("Guardian Engine Scene (*.gscene)\0*.gscene\0");
		}

		std::ofstream SceneFileOutput(this->CurrentScenePath);
		SceneFileOutput << SceneOutput.c_str();
	}

	void GuardianScene::SaveSceneAs(const GString& filePath)
	{
		YAML::Emitter SceneOutput;
		SceneOutput << YAML::BeginMap;
		SceneOutput << YAML::Key << "Scene";
		SceneOutput << YAML::Value << "Unnamed";
		SceneOutput << YAML::Key << "Entities";
		SceneOutput << YAML::Value << YAML::BeginSeq;
		for (auto& entity : this->SceneEntityList)
		{
			this->SaveEntity(SceneOutput, entity.second);
		}
		SceneOutput << YAML::EndSeq;
		SceneOutput << YAML::EndMap;

		this->CurrentScenePath = filePath;

		std::ofstream SceneFileOutput(this->CurrentScenePath);
		SceneFileOutput << SceneOutput.c_str();
	}

	void GuardianScene::UpdateEditScene(GuardianTimestep deltaTime)
	{
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

			while (!GuardianMouse::IsMouseRawMoveEventListEmpty())
			{
				GuardianMouseRawMoveEvent event = GuardianMouse::ReadRawMoveEvent();
				this->EditorCamera->Rotate(GVector3(event.MouseMovementY * 0.1f, event.MouseMovementX * 0.1f, 0.0f));
			}
		}

		if (!this->SceneGrid.get())
		{
			this->SceneGrid = GuardianModel::CreateNewModel(
				GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
				"../Guardian Engine/Resources/Models/Grid/Grid.obj");
		}
		this->SceneGrid->SubmitToRenderer();
		this->SceneGrid->UpdateModel(GuardianTransform().GetTransformMatrix() *
			this->EditorCamera->GetViewMatrix() * this->EditorCamera->GetProjectionMatrix());

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianCameraComponent>();
			view.each([this](const auto& e, 
				GuardianTransformComponent& TComponent, GuardianCameraComponent& CComponent)
			{
				CComponent.Position = TComponent.Position;
				CComponent.Direction = TComponent.Rotation;

				if (!CComponent.CameraModel.get())
				{
					CComponent.CameraModel = GuardianModel::CreateNewModel(
						GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
						"../Guardian Engine/Resources/Models/Camera/Camera.obj");
				}
				CComponent.CameraModel->SubmitToRenderer();

				CComponent.CameraModel->UpdateModel(TComponent.GetTransformMatrix() *
					this->EditorCamera->GetViewMatrix() *
					this->EditorCamera->GetProjectionMatrix());
			});
		}

		{
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianModelComponent>();
			view.each([this](const auto& e, 
				GuardianTransformComponent& TComponent, GuardianModelComponent& MComponent)
			{
					MComponent.UpdateModel(TComponent.GetTransformMatrix() * 
						this->EditorCamera->GetViewMatrix() * 
						this->EditorCamera->GetProjectionMatrix());

				MComponent.SubmitToRenderer();
			});
		}
	}

	void GuardianScene::UpdateRuntimeScene()
	{
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
			auto view = this->SceneRegistry.view<GuardianTransformComponent, GuardianModelComponent>();
			view.each([this](const auto& e,
				GuardianTransformComponent& TComponent, GuardianModelComponent& MComponent)
			{
					MComponent.UpdateModel(TComponent.GetTransformMatrix() *
						this->RuntimeCamera->GetViewMatrix() *
						this->RuntimeCamera->GetProjectionMatrix());

				MComponent.SubmitToRenderer();
			});
		}
	}

	void GuardianScene::StartRuntime()
	{
		this->SceneState = GE_SCENE_RUNTIME;

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

	void GuardianScene::StopRuntime()
	{
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

		if (entity->HasComponent<GuardianModelComponent>())
		{
			output << YAML::Key << "Model Component";
			output << YAML::BeginMap;

			output << YAML::Key << "File Path" << YAML::Value << 
				entity->GetComponent<GuardianModelComponent>().GetModelFilePath();

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
