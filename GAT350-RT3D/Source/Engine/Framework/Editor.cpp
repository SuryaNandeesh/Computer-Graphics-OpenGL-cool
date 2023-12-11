#include "Editor.h"
#include "Scene.h"

namespace nc
{
	void Editor::ProcessGui(Scene* scene)
	{
		// show resources
		ImGui::Begin("Resources");
		// models
		if (ImGui::CollapsingHeader("Textures"))
		{

			auto resources = GET_RESOURCES(Texture);
			for (auto& resource : resources)
			{
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) m_selected = resource.get();
			}
		}
		//models
		if (ImGui::CollapsingHeader("Models"))
		{
			auto resources = GET_RESOURCES(Model);
			for (auto& resource : resources)
			{
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) m_selected = resource.get();
			}
		}
		//shaders
		if (ImGui::CollapsingHeader("Shaders"))
		{
			auto resources = GET_RESOURCES(Shader);
			for (auto& resource : resources)
			{
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) m_selected = resource.get();
			}
		}
		//materials
		if (ImGui::CollapsingHeader("Materials"))
		{
			auto resources = GET_RESOURCES(Material);
			for (auto& resource : resources)
			{
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) m_selected = resource.get();
			}
		}

		ImGui::End();


		//show scene
		ImGui::Begin("Scene");
		scene->ProcessGui();
		ImGui::Separator();
		//show actors
		for (auto& actor : scene->m_actors)
		{
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();
			}
		}
		ImGui::End();

		//show inspector
		ImGui::Begin("Inspector");
		if (m_selected)
		{
			m_selected->ProcessGui();
			//delete selected actor
			if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				auto actor = dynamic_cast<Actor*>(m_selected);
				if (actor) {
					scene->Remove(actor);
					m_selected = nullptr;
				}
			}
		}
		
		ImGui::End();
	}
}
