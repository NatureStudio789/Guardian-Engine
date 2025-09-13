#include "GSceneEditor.h"

namespace GE
{
	void GSceneEditor::Initialize()
	{
		EUI::GWidget::Style NoPaddingStyle;
		NoPaddingStyle.Index = NoPaddingStyle.GEStyleVar_WindowPadding;
		NoPaddingStyle.Value = GVector2(0.0f, 0.0f);
		
		{
			this->SceneEditPanel = std::make_shared<EUI::GPanel>("Scene");
			this->SceneEditPanel->AddStyleToWidget(NoPaddingStyle);

			this->SceneEditImage = std::make_shared<EUI::GImage>((EUI::GImage::Id)GRenderer::GetSceneRenderGraph()->GetEditFramebuffer()->
				GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle.ptr, GVector2());
			this->SceneEditPanel->AddWidgetToPanel(this->SceneEditImage);

			this->EditManipulater = std::make_shared<EUI::GManipulater>();
			this->EditManipulater->DisableWidgetRendering();
			this->EditManipulater->SetCurrentOperation(EUI::GManipulater::GE_OPERATION_ROTATE);
			this->SceneEditPanel->AddWidgetToPanel(this->EditManipulater);

			this->SceneEditPanel->SetWidgetEventProcessFunction([=]()
				{
					this->EditManipulater->SetManipulateRect({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y }, {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetWindowContentRegionMin().y });
					this->SceneEditImage->SetImageSize({ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
				});

			this->AddWidgetToEditor(this->SceneEditPanel);
		}

		{
			this->SceneRuntimePanel = std::make_shared<EUI::GPanel>("Game");
			this->SceneRuntimePanel->AddStyleToWidget(NoPaddingStyle);

			this->SceneRuntimeImage = std::make_shared<EUI::GImage>((EUI::GImage::Id)GRenderer::GetSceneRenderGraph()->GetRuntimeFramebuffer()->
				GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle.ptr, GVector2());
			this->SceneRuntimePanel->AddWidgetToPanel(this->SceneRuntimeImage);
			this->SceneRuntimePanel->SetWidgetEventProcessFunction([=]()
			{
				this->SceneRuntimeImage->SetImageSize({ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
			});

			this->AddWidgetToEditor(this->SceneRuntimePanel);
		}

		{
			this->SceneHierarchyPanel = std::make_shared<EUI::GPanel>("Hierarchy");
			this->SceneHierarchyPanel->SetWidgetEventProcessFunction([=]()
			{
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				{
					this->SelectedEntityId = 0;
				}
			});

			this->AddWidgetToEditor(this->SceneHierarchyPanel);
		}

		{
			this->PropertiesPanel = std::make_shared<EUI::GPanel>("Properties");

			this->AddWidgetToEditor(this->PropertiesPanel);
		}
	}

	void GSceneEditor::Update()
	{
		GEditor::Update();

		this->UpdateEditPanel();
		this->UpdateHierarchyPanel();
		this->UpdatePropertiesPanel();
	}

	void GSceneEditor::UpdateEditPanel()
	{
		if (GSceneRegistry::GetActiveScene()->HasEntity(this->SelectedEntityId))
		{
			this->EditManipulater->EnableWidgetRendering();

			static XMFLOAT4X4 view;
			static XMFLOAT4X4 projection;
			XMStoreFloat4x4(&view, GSceneRegistry::GetActiveScene()->GetEditCamera()->GetViewMatrix());
			XMStoreFloat4x4(&projection, GSceneRegistry::GetActiveScene()->GetEditCamera()->Projection.GetProjectionMatrix());

			this->EditManipulater->SetViewMatrix((const float*)view.m);
			this->EditManipulater->SetProjectionMatrix((const float*)projection.m);

			static XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId)->GetComponent<GTransformComponent>().Transform.GetTransformMatrix());
			this->EditManipulater->SetTransformMatrix((float*)transform.m);

			this->EditManipulater->SetOperationProcessFunction([this](const EUI::GManipulater::Operation& operation)
				{
					if (ImGuizmo::IsUsing())
					{
						switch (operation)
						{
							case EUI::GManipulater::GE_OPERATION_TRANSLATE:
							{
								GVector3 tempr;
								GVector3 temps;
								GMatrix(XMLoadFloat4x4(&transform)).Decompose(GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId)->
									GetComponent<GTransformComponent>().Transform.Position,
									tempr, temps);

								break;
							}

							case EUI::GManipulater::GE_OPERATION_ROTATE:
							{
								GVector3 tempt;
								GVector3 temps;
								GMatrix(XMLoadFloat4x4(&transform)).Decompose(tempt,
									GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId)->
									GetComponent<GTransformComponent>().Transform.Rotation, temps);

								break;
							}

							case EUI::GManipulater::GE_OPERATION_SCALE:
							{
								GVector3 tempt;
								GVector3 tempr;
								GMatrix(XMLoadFloat4x4(&transform)).Decompose(tempt, tempr,
									GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId)->
									GetComponent<GTransformComponent>().Transform.Scale);

								break;
							}
						}
					}
				});
		}
		else
		{
			this->EditManipulater->DisableWidgetRendering();
		}
	}

	void GSceneEditor::UpdateHierarchyPanel()
	{
		this->SceneHierarchyPanel->ClearPanelWidgets();

		std::shared_ptr<EUI::GTreeNode> Root = std::make_shared<EUI::GTreeNode>(
			GSceneRegistry::GetActiveScene()->GetSceneName(), EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
		this->SceneHierarchyPanel->AddWidgetToPanel(Root);

		for (auto& child : GSceneRegistry::GetActiveScene()->GetSceneRootEntity()->GetChildren())
		{
			this->AddSceneNode(child, Root);
		}
	}

	void GSceneEditor::AddSceneNode(GEntity* entity, std::shared_ptr<EUI::GTreeNode> parent)
	{
		int NodeFlags = (this->SelectedEntityId == entity->GetEntityId()) ? EUI::GTreeNode::GETreeNodeFlags_Selected : EUI::GTreeNode::GETreeNodeFlags_None;
		std::shared_ptr<EUI::GTreeNode> node = std::make_shared<EUI::GTreeNode>(entity->GetEntityName(), NodeFlags);
		parent->AddWidgetToTreeNode(node);
		node->SetWidgetEventProcessFunction([=]()
		{
			if (ImGui::IsItemClicked())
			{
				this->SelectedEntityId = entity->GetEntityId();
			}
		});

		for (auto& child : entity->GetChildren())
		{
			this->AddSceneNode(child, node);
		}
	}

	void GSceneEditor::UpdatePropertiesPanel()
	{
		this->PropertiesPanel->ClearPanelWidgets();

		if (GSceneRegistry::GetActiveScene()->HasEntity(this->SelectedEntityId))
		{
			this->AddEntityProperties(GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId));
		}
	}

	void GSceneEditor::AddEntityProperties(std::shared_ptr<GEntity> entity)
	{
		GUARDIAN_CHECK_POINTER(entity);

		std::shared_ptr<EUI::GPopup> ComponentPopup = std::make_shared<EUI::GPopup>("Component List");

		if (entity->HasComponent<GTagComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Tag", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);
			
			auto& component = entity->GetComponent<GTagComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GInputBox>("Tag", &component.Tag));

			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GSeparator>());
		}
		else
		{
			ComponentPopup->AddWidgetToPopup(std::make_shared<EUI::GMenuItem>("Tag", [entity]()
			{
				entity->AddComponent<GTagComponent>();
			}));
		}

		if (entity->HasComponent<GTransformComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Transform", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GTransformComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Position", &component.Transform.Position, 0.1f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Rotation", &component.Transform.Rotation, 0.1f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Scale", &component.Transform.Scale, 0.1f));

			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GSeparator>());
		}
		else
		{
			ComponentPopup->AddWidgetToPopup(std::make_shared<EUI::GMenuItem>("Transform", [entity]()
				{
					entity->AddComponent<GTransformComponent>();
				}));
		}

		if (entity->HasComponent<GCameraComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Camera", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GCameraComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("FOV", &component.Camera->Projection.FOV, 0.1f, 30.0f, 100.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("NearZ", &component.Camera->Projection.NearZ, 0.001f, 0.01f, 1.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("FarZ", &component.Camera->Projection.FarZ, 0.1f, 1.0f));

			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GSeparator>());
		}
		else
		{
			ComponentPopup->AddWidgetToPopup(std::make_shared<EUI::GMenuItem>("Camera", [entity]()
				{
					entity->AddComponent<GCameraComponent>();
				}));
		}

		if (entity->HasComponent<GPointLightComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Point Light", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GPointLightComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Color", &component.PointLight->Color, 0.1f, 0.0f, 1.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Strength", &component.PointLight->Strength, 0.1f, 1.0f));

			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GSeparator>());
		}
		else
		{
			ComponentPopup->AddWidgetToPopup(std::make_shared<EUI::GMenuItem>("Point Light", [entity]()
				{
					entity->AddComponent<GPointLightComponent>();
				}));
		}

		if (entity->HasComponent<GModelComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Model", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GModelComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GText>("Static Mesh"));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GButton>(component.ModelAssetName, []() {}, GVector2(50.0f, 50.0f)));

			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GSeparator>());
		}
		else
		{
			ComponentPopup->AddWidgetToPopup(std::make_shared<EUI::GMenuItem>("Model", [entity]()
				{
					entity->AddComponent<GModelComponent>().ModelAssetName = "Box";
				}));
		}

		this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GButton>("Add Component",
			[ComponentPopup]()
			{
				ComponentPopup->ActivePopup();
			}, GVector2(150.0f, 40.0f)));

		this->PropertiesPanel->AddWidgetToPanel(ComponentPopup);
	}
}
