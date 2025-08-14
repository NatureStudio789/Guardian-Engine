#include "GDockspace.h"
#include "../../Editor/GEditor.h"

namespace GE
{
	namespace EUI
	{
		GDockspace::GDockspace(const std::string& name)
		{
			this->SetDockspaceName(name);
		}

		void GDockspace::SetDockspaceName(const std::string& name)
		{
			this->DockspaceName = name;
		}

		void GDockspace::AddEditorToDockspace(std::shared_ptr<GEditor> editor)
		{
			this->DockspaceEditorList.push_back(editor);
		}

		void GDockspace::ClearDockspaceEditors()
		{
			this->DockspaceEditorList.clear();
		}

		void GDockspace::Update()
		{
			for (int i = 0; i < (int)this->DockspaceEditorList.size(); i++)
			{
				this->DockspaceEditorList[i]->Update();
			}
		}

		void GDockspace::Render()
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f });
			ImGui::Begin((this->DockspaceName + " Window").c_str(), nullptr, window_flags);
			ImGui::PopStyleColor();
			ImGui::PopStyleVar(3);

			ImGui::DockSpace(ImGui::GetID(this->DockspaceName.c_str()));

			for (auto& editor : this->DockspaceEditorList)
			{
				editor->Render();
			}

			ImGui::End();
		}

		const std::string& GDockspace::GetDockspaceName() const noexcept
		{
			return this->DockspaceName;
		}
	}
}
